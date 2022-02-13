// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Task.h"
#include "DialogSubsystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BaseAi.generated.h"

class UDataTable;
class AAIController;
class AStaticMeshActor;
class UParticleSystemComponent;
class UAudioComponent;

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

	UPROPERTY()
	AActor* MoveTarget;

	void ContinueCurrentTask();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , meta = (RequiredAssetDataTags = "RowStructure=BotTask"))
	UDataTable* AllAvailableTasksDT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> SelectedTasks;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UAudioComponent* AudioComponent;

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

	bool bCanMove = true;

	FBotTask* GetNextTask();

	void StartTaskAction();

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION()
	void OnBlockedMovement();

	UFUNCTION()
	void OnTaskCompleted();

	void GoToNextTask();

	virtual void TaskCompleted() {};
	virtual void TaskStarted() {};

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


};
