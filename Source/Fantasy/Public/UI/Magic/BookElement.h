// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MagicComponent.h"
#include "BookElement.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UBookOfMagic;

UCLASS()
class FANTASY_API UBookElement : public UUserWidget
{
	GENERATED_BODY()

public:

	static UBookElement* Create(const FSpell& Spell, UBookOfMagic* Parent, TSubclassOf<UBookElement> WidgetClass);

protected:

	void SetInfo(const FSpell& Spell);

	UFUNCTION()
	void OnSpellInfoSelected();

	UPROPERTY()
	UBookOfMagic* ParentWidget;

	UPROPERTY(BlueprintReadOnly)
	FSpell CurrentSpell;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* SpellImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ShowInfo;

};
