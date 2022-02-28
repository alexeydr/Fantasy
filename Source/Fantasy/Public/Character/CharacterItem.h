// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SaleableInterface.h"
#include "Engine/DataTable.h"
#include "Interfaces/InteractionInterface.h"
#include "CharacterItem.generated.h"

class ACharacterItem;

USTRUCT(BlueprintType)
struct FCharacterItemStruct : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacterItem> ItemClass;

};

UCLASS(BlueprintType)
class USalableItem : public UObject, public ISaleableInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FCharacterItemStruct ItemInfo;

	FText GetSalableItemName_Implementation();

	FText GetSalableItemDesc_Implementation();

	class UTexture2D* GetSalableItemTexture_Implementation();

	float GetSalableItemPrice_Implementation();

	void OnItemSaled_Implementation(UObject* WCO);

	bool CheckAlreadyBought_Implementation(UObject* WCO);
};

UCLASS()
class FANTASY_API ACharacterItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	

	ACharacterItem();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LastingEffectTime = 1.f;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LastingEffect();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EquipedEffect();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UnEquipedEffect();

public:	

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteraction();

};
