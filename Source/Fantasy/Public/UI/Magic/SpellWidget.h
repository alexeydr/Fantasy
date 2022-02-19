// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MagicComponent.h"
#include "SpellWidget.generated.h"

class UImage;
class UTexture2D;
class UTextBlock;

UCLASS()
class FANTASY_API USpellWidget : public UUserWidget
{
	GENERATED_BODY()
	
public: 

	void InitializeWidget(const FSpell& Spell, int SpellNumber);

protected:

	UFUNCTION()
	void UpdateActiveSpell(ESpellStatus Status, const FSpell& Spell);

	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* SpellImage;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Number;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* InactiveTexture;

	UPROPERTY()
	FSpell ActiveSpell;

	UPROPERTY()
	UMagicComponent* MagicComp;

};
