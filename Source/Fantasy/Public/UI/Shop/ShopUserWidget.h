// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/SaleableInterface.h"
#include "ShopUserWidget.generated.h"

class UScrollBox;
class UShopElement;

UCLASS()
class FANTASY_API UShopUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CreateAssortment(const TArray<FSalebleItems>& Assortment);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* ItemsContainer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UShopElement> ShopElementClass;

};
