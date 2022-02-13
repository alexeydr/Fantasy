// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/SaleableInterface.h"
#include "ShopElement.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UStatsComponent;

UCLASS()
class FANTASY_API UShopElement : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetAlreadyBoughtStatus();
	void CreateSaleableItem(const FSalebleItems& InItem);
	virtual void NativeConstruct();

protected:

	UFUNCTION()
	void OnBuyItemClicked();

	UFUNCTION()
	void OnMoneyChanged(float NewMoney);

	UPROPERTY()
	UStatsComponent* StatsComp;

	UPROPERTY()
	FSalebleItems Item;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemDesc;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemPriceTB;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* AlreadyBought;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BuyItem;

	float ItemPrice;

};
