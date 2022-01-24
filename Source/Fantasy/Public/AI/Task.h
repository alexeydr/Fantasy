// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Animation/AnimSequence.h"
#include "Task.generated.h"

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
	bool bNeedChangePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangePosition"))
	FVector CharLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangePosition"))
	FRotator CharRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedInteractWithMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedInteractWithMesh"))
	UStaticMesh* MeshForInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedChangeMeshPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangeMeshPosition"))
	FRotator MeshRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangeMeshPosition"))
	FVector AdditionalLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedChangeMeshPosition"))
	FVector NewScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttachMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedSpawnParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedSpawnParticle"))
	UParticleSystem* EmitterTemplate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedSpawnParticle"))
	FVector EmitterLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedSpawnParticle"))
	FRotator EmitterRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedSpawnParticle"))
	FVector EmitterScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedEmoji;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bNeedEmoji"))
	UTexture2D* EmojiTexture;

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

	bool bIsEmptyTask = true;

protected:

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
