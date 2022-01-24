// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Task.h"
#include "Navigation/PathFollowingComponent.h"
#include "FriendlyAi.generated.h"

class UDataTable;
class AFriendlyAIController;
class AStaticMeshActor;
class UParticleSystemComponent;
class UWidgetComponent;
class UEmojiComponent;

UCLASS()
class FANTASY_API AFriendlyAi : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFriendlyAi();

	UFUNCTION(BlueprintPure)
	const FBotTask& GetCurrentTask() { return CurrentTask; };

	UPROPERTY(BlueprintReadOnly)
	bool bTaskStarted;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta = (RequiredAssetDataTags = "RowStructure=BotTask"))
	UDataTable* AllAvailableTasksDT;

	UPROPERTY(EditAnywhere)
	TArray<FString> SelectedTasks;

	UPROPERTY()
	FBotTask CurrentTask;

	UPROPERTY()
	ATask* CurrentTaskActor;

	UPROPERTY()
	AFriendlyAIController* AiController;

	UPROPERTY()
	AStaticMeshActor* TaskMesh;

	UPROPERTY()
	UParticleSystemComponent* EmitterComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UWidgetComponent* WidgetComp;

	UPROPERTY()
	UEmojiComponent* EmojiComp;

	FBotTask* GetNextTask();

	bool GoToNextTask(); //true if success

	void StartTaskAction();

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION()
	void OnTaskCompleted();

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
