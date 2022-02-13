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
class UAnimSequence;

UENUM(BlueprintType)
enum class EAiState : uint8
{
	None,
	Block,
	Attack,
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveTargetDistance;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
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
	UAnimSequence* AttackAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequence* BlockAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequence* HitAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	bool bCanAttack = true;

	virtual void AttackResult() {};
	virtual void GoToAttackPosition() {};
	virtual void DoDamage(float InDamage) {};

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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
