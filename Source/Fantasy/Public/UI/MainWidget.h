// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Character/MainCharacter.h"
#include "MainWidget.generated.h"

UCLASS()
class FANTASY_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()


protected:

	UFUNCTION()
	void UpdateMoney(float NewMoneyValue);

	UFUNCTION()
	void OnDestroyedPlayer(AActor* DestroyedActor);

	UFUNCTION()
	void OnHealthChanged(float NewHealth);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY()
	AMainCharacter* MainChar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MoneyText;

};
