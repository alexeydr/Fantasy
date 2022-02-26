// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMagicComponent;
class UStatsComponent;
class UChildActorComponent;
class UInteractionComponent;
class ACharacterItem;

UCLASS()
class FANTASY_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMainCharacter();

protected:

	UPROPERTY()
	ACharacterItem* AttachedActor;

	UPROPERTY()
	AActor* InteractionActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMagicComponent* MagicComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStatsComponent* StatsComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void OnCharacterInteraction();
	
	template<int SpellNumber>
	void CastSpellTemplate()
	{
		CastSpell(SpellNumber);
	};

	void CastSpell(int Number);

public:

	bool IsHasShield();

	void SetInteractionActor(AActor* NewActor) { InteractionActor = NewActor; };

	UFUNCTION(BlueprintPure)
	AActor* GetInteractionActor() const	{ return InteractionActor; };

	UFUNCTION(BlueprintPure)
	ACharacterItem* GetAttachedActor() const { return AttachedActor; };

	UFUNCTION(BlueprintPure)
	UMagicComponent* GetMagicComponent() const { return MagicComp; };

	UFUNCTION(BlueprintPure)
	UStatsComponent* GetStatsComponent() const { return StatsComp; };
	
	UFUNCTION(BlueprintPure)
	UCameraComponent* GetCameraComponent() const { return FollowCamera; };

	void EquipNewItem(ACharacterItem* NewEquipedItem);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
