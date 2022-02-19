// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Magic/BookElement.h"
#include "UI/Magic/BookSpellStat.h"
#include "Components/MagicComponent.h"
#include "BookOfMagic.generated.h"

class UMagicComponent;
class UScrollBox;
class UOverlay;

UCLASS()
class FANTASY_API UBookOfMagic : public UUserWidget
{
	GENERATED_BODY()

public:

	void OnSpellSelected(const FSpell& SelectedSpell);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* SpellsContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* PropertyContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBookElement> ElementWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBookSpellStat> StatWidgetClass;

	UPROPERTY()
	UMagicComponent* MagicComp;

	UFUNCTION()
	void CreateSpells();

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
};
