// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Animation/AnimSequence.h"
#include "Task.generated.h"

class ABaseAi;
class USoundBase;

USTRUCT(BlueprintType)
struct FRandomParticles : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* EmitterTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EmitterScale = FVector(1.f,1.f,1.f);

};

USTRUCT(BlueprintType)
struct FBotTask : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsHomeTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TaskTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SoundForTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedChangePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangePosition", EditConditionHides))
	FVector CharLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangePosition", EditConditionHides))
	FRotator CharRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedInteractWithMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedInteractWithMesh", EditConditionHides))
	UStaticMesh* MeshForInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedChangeMeshPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangeMeshPosition", EditConditionHides))
	FRotator MeshRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangeMeshPosition", EditConditionHides))
	FVector AdditionalLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangeMeshPosition", EditConditionHides))
	FVector NewScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttachMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedSpawnParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedSpawnParticle", EditConditionHides))
	UParticleSystem* EmitterTemplate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedSpawnParticle", EditConditionHides))
	FVector EmitterLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedSpawnParticle", EditConditionHides))
	FRotator EmitterRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedSpawnParticle", EditConditionHides))
	FVector EmitterScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedEmoji;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedEmoji", EditConditionHides))
	UTexture2D* EmojiTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawnRandomParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RequiredAssetDataTags = "RowStructure=RandomParticles", EditCondition = "bSpawnRandomParticle", EditConditionHides))
	UDataTable* AllRandomParticlesDT;

	bool operator==(const FBotTask& BotTask)
	{
		return Name == BotTask.Name;
	}

};

UCLASS()
class FANTASY_API ATask : public AActor
{
	GENERATED_BODY()
	
public:	

	ATask();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CurrentTaskName;

	UPROPERTY()
	bool bIsEmptyTask = true;


protected:

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
