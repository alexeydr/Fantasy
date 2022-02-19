// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MagicComponent.h"
#include "BookSpellStat.generated.h"

class UTexture2D;
class UImage;
class UTextBlock;
class UWorld;

UCLASS()
class FANTASY_API UBookSpellStat : public UUserWidget
{
	GENERATED_BODY()

public:

	static UBookSpellStat* Create(UWorld* World, const FSpell& Spell, TSubclassOf<UBookSpellStat> WidgetClass);

protected:

	void SetInfo(const FSpell& Spell);

	UPROPERTY(BlueprintReadOnly)
	FSpell CurrentSpell;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* SpellImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Damage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ManaPrice;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Type;
};
