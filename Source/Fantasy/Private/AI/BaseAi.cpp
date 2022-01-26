#include "AI/BaseAi.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseAi::ABaseAi()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->SetUpdateNavAgentWithOwnersCollisions(true);
	GetCharacterMovement()->UpdateNavAgent(*GetCapsuleComponent());
}

// Called when the game starts or when spawned
void ABaseAi::BeginPlay()
{
	Super::BeginPlay();

	AiController = Cast<AAIController>(GetController());
	if (AiController)
	{
		AiController->ReceiveMoveCompleted.AddDynamic(this, &ThisClass::OnMoveCompleted);
		GoToNextTask();
	}
}

FBotTask* ABaseAi::GetNextTask()
{
	int CurrentTaskNumber = SelectedTasks.Find(CurrentTask.Name);
	FString CurrentTaskName = (SelectedTasks.Num() - 1 == CurrentTaskNumber || CurrentTaskNumber < 0) ? SelectedTasks[0] : SelectedTasks[CurrentTaskNumber + 1];

	FString ContextString;
	TArray<FName> RowNames;
	RowNames = AllAvailableTasksDT->GetRowNames();
	for (auto& name : RowNames)
	{
		FBotTask* row = AllAvailableTasksDT->FindRow<FBotTask>(name, ContextString);
		if (row && CurrentTaskName == row->Name)
		{
			return row;
		}
	}
	return nullptr;
}

void ABaseAi::ContinueCurrentTask()
{
	if (AiController && CurrentTaskActor)
	{
		AiController->MoveTo(CurrentTaskActor);
		return;
	}
	GoToNextTask();
}

void ABaseAi::GoToNextTask()
{
	auto* NextTask = GetNextTask();
	if (AiController && NextTask)
	{
		float NearestPoint = FLT_MAX;
		ATask* NearestTask = nullptr;
		for (TActorIterator<ATask> TaskItr(GetWorld()); TaskItr; ++TaskItr)
		{
			if (TaskItr->CurrentTaskName == NextTask->Name && TaskItr->bIsEmptyTask && NearestPoint > TaskItr->GetSquaredDistanceTo(this))
			{
				NearestTask = *TaskItr;
				NearestPoint = TaskItr->GetSquaredDistanceTo(this);
			}
		}
		if (NearestTask)
		{
			CurrentTask = *NextTask;
			CurrentTaskActor = NearestTask;
			CurrentTaskActor->bIsEmptyTask = false;
			AiController->MoveToActor(CurrentTaskActor);
			return;
		}
		CurrentTask = *NextTask;
		GoToNextTask();
	}
}

void ABaseAi::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (!MoveTarget)
	{
		if (Result == EPathFollowingResult::Success)
		{
			StartTaskAction();
		}

		if (Result == EPathFollowingResult::Blocked)
		{
			GoToNextTask();
		}
	}
	
}

void ABaseAi::StartTaskAction()
{
	bTaskStarted = true;
	if (CurrentTaskActor)
	{
		SetActorRotation(CurrentTaskActor->GetActorRotation());
	}

	if (CurrentTask.bNeedChangePosition)
	{
		SetActorLocationAndRotation(CurrentTask.CharLocation, CurrentTask.CharRotation);
	}

	if (CurrentTask.bIsHomeTask)
	{
		if (TaskMesh)
		{
			TaskMesh->Destroy();
		}
		GetMesh()->SetVisibility(false);
	}

	if (CurrentTask.bNeedSpawnParticle)
	{
		if (EmitterComponent)
		{
			EmitterComponent->DestroyComponent();
		}
		EmitterComponent = UGameplayStatics::SpawnEmitterAtLocation(this, CurrentTask.EmitterTemplate, CurrentTask.EmitterLocation, CurrentTask.EmitterRotation, CurrentTask.EmitterScale);
	}

	TaskStarted();

	FTimerHandle Th;
	GetWorld()->GetTimerManager().SetTimer(Th, this, &ABaseAi::OnTaskCompleted, CurrentTask.TaskTime, false);
}

void ABaseAi::OnTaskCompleted()
{
	TaskCompleted();

	if (CurrentTask.bNeedInteractWithMesh)
	{
		if (TaskMesh)
		{
			TaskMesh->Destroy();
		}
		TaskMesh = GetWorld()->SpawnActor<AStaticMeshActor>();
		if (TaskMesh)
		{
			TaskMesh->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			TaskMesh->GetStaticMeshComponent()->SetStaticMesh(CurrentTask.MeshForInteract);
			TaskMesh->SetActorEnableCollision(false);
			TaskMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("LeftHand"));

		}
	}

	if (TaskMesh)
	{
		if (CurrentTask.bNeedChangeMeshPosition)
		{
			TaskMesh->SetActorRotation(CurrentTask.MeshRotation);
			TaskMesh->AddActorLocalOffset(CurrentTask.AdditionalLocation);
			TaskMesh->SetActorScale3D(CurrentTask.NewScale);
		}

		if (!CurrentTask.bAttachMesh)
			TaskMesh->Destroy();
	}

	if (CurrentTask.bIsHomeTask)
		GetMesh()->SetVisibility(true);

	if (EmitterComponent)
		EmitterComponent->DestroyComponent();

	if (CurrentTaskActor)
		CurrentTaskActor->bIsEmptyTask = true;

	bTaskStarted = false;
	GoToNextTask();
}

void ABaseAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AiController && MoveTarget)
	{
		AiController->MoveToActor(MoveTarget);		
	}
}