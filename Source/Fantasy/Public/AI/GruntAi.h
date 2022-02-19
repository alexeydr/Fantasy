// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyAi.h"
#include "GruntAi.generated.h"

class UAnimSequence;

UCLASS()
class FANTASY_API AGruntAi : public AEnemyAi
{
	GENERATED_BODY()

protected: 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AdditionalAttackDistance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AdditionalDamage = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimationAsset* AdditionalAttackAnim;

	UPROPERTY(BlueprintReadOnly)
	bool bIsMainAttack = true;

	virtual void Tick(float DeltaTime) override;
	virtual void AttackResult() override;
	virtual void GoToAttackPosition() override;
	virtual UAnimationAsset* SelectAttack() override;
};
