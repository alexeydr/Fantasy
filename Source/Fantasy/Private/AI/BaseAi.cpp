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
#include "Components/AudioComponent.h"

// Sets default values
ABaseAi::ABaseAi()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->SetUpdateNavAgentWithOwnersCollisions(true);
	GetCharacterMovement()->UpdateNavAgent(*GetCapsuleComponent());
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComp");
}

// Called when the game starts or when spawned
void ABaseAi::BeginPlay()
{
	Super::BeginPlay();

	AiController = Cast<AAIController>(GetController());
	if (AiController && AllAvailableTasksDT)
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
		if (CurrentTaskActor)
		{
			CurrentTaskActor->bIsEmptyTask = true;
		}

		float NearestPoint = FLT_MAX;
		ATask* NearestTask = nullptr;
		for (TActorIterator<ATask> TaskItr(GetWorld()); TaskItr; ++TaskItr)
		{
			if (TaskItr->CurrentTaskName == NextTask->Name && TaskItr->bIsEmptyTask && NearestPoint > TaskItr->GetDistanceTo(this))
			{
				NearestTask = *TaskItr;
				NearestPoint = TaskItr->GetDistanceTo(this);
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
		FTimerHandle Th;
		GetWorld()->GetTimerManager().SetTimer(Th, this, &ABaseAi::OnBlockedMovement, 0.5f, false);
	}
}

void ABaseAi::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (!MoveTarget)
	{
		UE_LOG(LogTemp, Error, TEXT("%s NameBot %i move result"),*GetName(), Result);
		if (Result == EPathFollowingResult::Success)
		{
			StartTaskAction();
		}

		if (Result == EPathFollowingResult::Blocked)
		{
			FTimerHandle Th;
			GetWorld()->GetTimerManager().SetTimer(Th, this, &ABaseAi::OnBlockedMovement, 0.5f, false);
			
		}
	}
	
}

void ABaseAi::OnBlockedMovement()
{
	GoToNextTask();
}

void ABaseAi::StartTaskAction()
{
	bTaskStarted = true;

	if (AudioComponent)
	{
		AudioComponent->SetSound(CurrentTask.SoundForTask);
		AudioComponent->SetActive(true);
		AudioComponent->Play();
	}

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
		GetMesh()->SetVisibility(false, true);
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

	if (AudioComponent)
	{
		AudioComponent->SetSound(nullptr);
		AudioComponent->SetActive(false);
		AudioComponent->Stop();
	}

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
		GetMesh()->SetVisibility(true, true);

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
	
	if (AiController && MoveTarget && bCanMove)
	{
		AiController->MoveToActor(MoveTarget);		
	}
}