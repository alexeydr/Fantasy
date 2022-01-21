// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "MagicComponent.generated.h"

UENUM(BlueprintType)
enum class ESpellStatus : uint8
{
	StartedCast     UMETA(DisplayName = "Started"),
	CastedCompleted      UMETA(DisplayName = "Casted"),
	CooldownCompleted UMETA(DisplayName = "Cooldown Completed")

};

USTRUCT(BlueprintType)
struct FSpell : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CastTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorToSpawn; //null if nothing needs to be spawned

	UPROPERTY()
	bool bIsActiveCooldown;

	bool operator==(const FSpell& Spell)
	{
		return Name.ToString() == Spell.Name.ToString();
	}

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpellsStatusChanged, ESpellStatus, Status, const FSpell&, Spell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaValueChanged, float, NewMana);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASY_API UMagicComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UMagicComponent();

	UFUNCTION(BlueprintCallable)
	void DecreaseMana(float Value);

	UFUNCTION(BlueprintCallable)
	void AddMana(float Value);

	UFUNCTION(BlueprintCallable)
	float GetMana() const {	return Mana; };

	UFUNCTION(BlueprintCallable)
	bool IsCanCastSpell() const { return bCanCastSpell;	};

	UFUNCTION(BlueprintCallable)
	const TArray<FSpell>& GetActiveSpells() const { return ActiveSpells; };

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnManaValueChanged OnManaValueChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnSpellsStatusChanged OnSpellsStatusChanged;

protected:

	void AddSpellsFromDT();

	virtual void BeginPlay() override;

	void CastSpell(const FSpell& Spell);

	void PrepareForCastSpell(FSpell& Spell);

	UFUNCTION()
	void OnSpellCasted(const FSpell& SpellCompletedCast);

	UFUNCTION()
	void OnSpellCooldownCompleted(int Number);

	UPROPERTY(EditAnywhere, meta = (RequiredAssetDataTags = "RowStructure=Spell"))
	UDataTable* AvailableSpells;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	float Mana = 100.f;

	UPROPERTY()
	TArray<FSpell> ActiveSpells;

	TArray<FSpell*> CooldownSpells;

	bool bCanCastSpell;

public:	

	void PrepareForCastSpell(int SpellNumber);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
