// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MainWidget.h"
#include "Components/MagicComponent.h"
#include "MainMagicWidget.generated.h"

class UHorizontalBox;
class USpellWidget;

UCLASS()
class FANTASY_API UMainMagicWidget : public UMainWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	void UpdateMana(float NewManaValue);

	void CreateActiveSpells(const TArray<FSpell>& Spells);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* SpellsBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ManaText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USpellWidget> SpellWidgetClass;

};
