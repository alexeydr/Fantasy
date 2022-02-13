// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaleableInterface.generated.h"

class ISaleableInterface;

USTRUCT(BlueprintType)
struct FSalebleItems
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<ISaleableInterface> ItemInterface;

};

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class USaleableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FANTASY_API ISaleableInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnItemSaled(UObject* WCO);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetSalableItemName();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetSalableItemDesc();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UTexture2D* GetSalableItemTexture();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetSalableItemPrice();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckAlreadyBought(UObject* WCO);

};
