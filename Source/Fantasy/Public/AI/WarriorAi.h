// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyAi.h"
#include "WarriorAi.generated.h"

/**
 * 
 */
UCLASS()
class FANTASY_API AWarriorAi : public AEnemyAi
{
	GENERATED_BODY()

protected:
	virtual void AttackResult() override;
	virtual void GoToAttackPosition() override;
	virtual void DoDamage(float InDamage) override;
};
