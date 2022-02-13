// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Interfaces/SaleableInterface.h"
#include "MagicComponent.generated.h"

class USoundBase;

UENUM(BlueprintType)
enum class ESpellType : uint8
{
	Projectile     UMETA(DisplayName = "Projectile"),
	Shield      UMETA(DisplayName = "Shield"),
	Zone UMETA(DisplayName = "Zone")

};

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
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorToSpawn; //null if nothing needs to be spawned
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpellType TypeSpell;

	//https://benui.ca/unreal/uproperty-edit-condition-can-edit-change/  --- hide property if TypeSpell != Zone
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D ZoneSize;

	UPROPERTY()
	bool bIsActiveCooldown;

	friend bool operator==(const FSpell& SpellL, const FSpell& SpellR)
	{
		return SpellL.Name.ToString() == SpellR.Name.ToString();
	}


};

UCLASS(BlueprintType)
class USalableSpell : public UObject, public ISaleableInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FSpell SpellForSale;

	FText GetSalableItemName_Implementation();

	FText GetSalableItemDesc_Implementation();

	class UTexture2D* GetSalableItemTexture_Implementation();

	float GetSalableItemPrice_Implementation();

	void OnItemSaled_Implementation(UObject* WCO);

	bool CheckAlreadyBought_Implementation(UObject* WCO);
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpellsCountChanged);
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

	UFUNCTION(BlueprintPure)
	float GetMana() const {	return Mana; };

	UFUNCTION(BlueprintCallable)
	bool IsCanCastSpell() const { return bCanCastSpell;	};

	UFUNCTION(BlueprintPure)
	float GetCurrentCastSpell() const { return CurrentCastSpell; };

	UFUNCTION(BlueprintPure)
	const TArray<FSpell>& GetActiveSpells() const { return ActiveSpells; };

	UFUNCTION(BlueprintCallable)
	void AddNewSpell(const FSpell& NewSpell)
	{
		ActiveSpells.Add(NewSpell);

		if (OnSpellsCountChanged.IsBound())
			OnSpellsCountChanged.Broadcast();
	};

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnManaValueChanged OnManaValueChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnSpellsStatusChanged OnSpellsStatusChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnSpellsCountChanged OnSpellsCountChanged;
	

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
	float CurrentCastSpell;

public:	

	void PrepareForCastSpell(int SpellNumber);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
