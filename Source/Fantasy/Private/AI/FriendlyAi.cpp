#include "AI/FriendlyAi.h"
#include "AI/FriendlyAIController.h"
#include "EngineUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Texture2D.h"
#include "Components/WidgetComponent.h"
#include "Components/EmojiComponent.h"

AFriendlyAi::AFriendlyAi()
{
	PrimaryActorTick.bCanEverTick = true;
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	EmojiComp = CreateDefaultSubobject<UEmojiComponent>("EmojiComponent");
}

void AFriendlyAi::BeginPlay()
{
	Super::BeginPlay();

	AiController = Cast<AFriendlyAIController>(GetController());
	if (AiController)
	{
		AiController->ReceiveMoveCompleted.AddDynamic(this, &AFriendlyAi::OnMoveCompleted);
		GoToNextTask();
	}

}

FBotTask* AFriendlyAi::GetNextTask()
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

bool AFriendlyAi::GoToNextTask()
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

void AFriendlyAi::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		StartTaskAction();
	}
}

void AFriendlyAi::StartTaskAction()
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

	if (CurrentTask.bNeedEmoji)
	{
		EmojiComp->WatchEmoji();
		EmojiComp->SetEmojiWidget(CurrentTask.EmojiTexture);
	}

	FTimerHandle Th;
	GetWorld()->GetTimerManager().SetTimer(Th, this, &AFriendlyAi::OnTaskCompleted, CurrentTask.TaskTime, false);
}

void AFriendlyAi::OnTaskCompleted()
{
	EmojiComp->HideEmoji();

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

void AFriendlyAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFriendlyAi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

