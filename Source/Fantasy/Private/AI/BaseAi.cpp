#include "AI/BaseAi.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseAi::ABaseAi()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseAi::BeginPlay()
{
	Super::BeginPlay();

	AiController = Cast<AAIController>(GetController());
	if (AiController)
	{
		AiController->ReceiveMoveCompleted.AddDynamic(this, &ABaseAi::OnMoveCompleted);
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

bool ABaseAi::GoToNextTask()
{
	auto* NextTask = GetNextTask();
	if (AiController && NextTask)
	{
		for (TActorIterator<ATask> TaskItr(GetWorld()); TaskItr; ++TaskItr)
		{
			if (TaskItr->CurrentTaskName == NextTask->Name && TaskItr->bIsEmptyTask)
			{
				CurrentTask = *NextTask;
				CurrentTaskActor = *TaskItr;
				TaskItr->bIsEmptyTask = false;
				AiController->MoveToActor(*TaskItr);
				return true;
			}
		}
	}
	return false;
}

void ABaseAi::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		StartTaskAction();
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
		GetMesh()->SetVisibility(false);
	}

	if (CurrentTask.bNeedSpawnParticle)
	{
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
		TaskMesh = GetWorld()->SpawnActor<AStaticMeshActor>();
		if (TaskMesh)
		{
			TaskMesh->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			TaskMesh->GetStaticMeshComponent()->SetStaticMesh(CurrentTask.MeshForInteract);
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
	if (!GoToNextTask())
	{
		CurrentTaskActor->bIsEmptyTask = false;
		StartTaskAction();
	}
}

