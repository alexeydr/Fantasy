// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAi.h"
#include "FriendlyAi.generated.h"

class UWidgetComponent;
class UEmojiComponent;
class UDialogComponent;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS()
class FANTASY_API AFriendlyAi : public ABaseAi
{
	GENERATED_BODY()

public:
	AFriendlyAi();

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UWidgetComponent* WidgetComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UEmojiComponent* EmojiComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UDialogComponent* DialogComp;

	UPROPERTY()
	UParticleSystemComponent* RandomParticleComp;

	virtual void TaskCompleted() override;
	virtual void TaskStarted() override;
};
