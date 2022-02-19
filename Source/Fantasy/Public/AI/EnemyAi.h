// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAi.h"
#include "Components/MagicComponent.h"
#include "EnemyAi.generated.h"

class UStatsComponent;
class UAIPerceptionComponent;
class AMainCharacter;
class USphereComponent;
class UAnimationAsset;

UENUM(BlueprintType)
enum class EAiState : uint8
{
	None,
	Block,
	Attack,
	Dead,
	Hit
};

UCLASS()
class FANTASY_API AEnemyAi : public ABaseAi
{
	GENERATED_BODY()

public:

	AEnemyAi();

	void ChangeFightModeStatus(bool bNewStatus);

	UFUNCTION(BlueprintPure)
	bool GetFightModeStatus() { return bFightModeActive; };

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStatsComponent* StatsComponent;
	
	UPROPERTY()
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY()
	USphereComponent* SphereComp;

	UPROPERTY()
	AMainCharacter* MainChar;

	UPROPERTY()
	UCharacterMovementComponent* MovementComp;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
	UFUNCTION()
	void OnCharacterCastSpell(ESpellStatus Status, const FSpell& Spell);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimationAsset* AttackAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimationAsset* BlockAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimationAsset* HitAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimationAsset* DeadAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackAngle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDistance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	bool bCanAttack = true;

	virtual void AttackResult() {};
	virtual void GoToAttackPosition() {};
	virtual UAnimationAsset* SelectAttack() {	return AttackAsset;	};

	UFUNCTION()
	void OnDeath();

	UFUNCTION(BlueprintCallable)
	void OnDidDamage(float InDamage);

	UFUNCTION()
	void OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly)
	bool bFightModeActive;

	EAiState CurrentState = EAiState::None;

	void Attack();
	void DodgeMagic();
	bool IsCharacterInAttackRadius();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
