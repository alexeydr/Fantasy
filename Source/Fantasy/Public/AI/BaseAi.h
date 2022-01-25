// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Task.h"
#include "Navigation/PathFollowingComponent.h"
#include "BaseAi.generated.h"

class UDataTable;
class AAIController;
class AStaticMeshActor;
class UParticleSystemComponent;

UCLASS()
class FANTASY_API ABaseAi : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseAi();

	UFUNCTION(BlueprintPure)
	const FBotTask& GetCurrentTask() { return CurrentTask; };

	UPROPERTY(BlueprintReadOnly)
	bool bTaskStarted;

protected:

	UPROPERTY(EditDefaultsOnly, meta = (RequiredAssetDataTags = "RowStructure=BotTask"))
	UDataTable* AllAvailableTasksDT;

	UPROPERTY(EditAnywhere)
	TArray<FString> SelectedTasks;

	UPROPERTY()
	FBotTask CurrentTask;

	UPROPERTY()
	ATask* CurrentTaskActor;

	UPROPERTY()
	AAIController* AiController;

	UPROPERTY()
	AStaticMeshActor* TaskMesh;

	UPROPERTY()
	UParticleSystemComponent* EmitterComponent;

	FBotTask* GetNextTask();

	bool GoToNextTask(); //true if success

	void StartTaskAction();

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION()
	void OnTaskCompleted();

	virtual void TaskCompleted() {};

	virtual void TaskStarted() {};

public:

	virtual void BeginPlay() override;


};
