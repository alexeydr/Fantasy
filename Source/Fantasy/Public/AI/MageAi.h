// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyAi.h"
#include "MageAi.generated.h"

UCLASS()
class FANTASY_API AMageAi : public AEnemyAi
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> MagicActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> IllusionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MagicDelay;

	UPROPERTY(BlueprintReadOnly)
	AActor* SpawnedActor;

	UPROPERTY(BlueprintReadOnly)
	AActor* SpawnedIllusuion;

	UFUNCTION()
	void OnMagicDelayCompleted();

	void SpawnIllusion();

	virtual void Tick(float DeltaTime) override;
	virtual void AttackResult() override;
	virtual void GoToAttackPosition() override;
};
