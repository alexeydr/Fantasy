// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAi.h"
#include "FriendlyAi.generated.h"

class UWidgetComponent;
class UEmojiComponent;

UCLASS()
class FANTASY_API AFriendlyAi : public ABaseAi
{
	GENERATED_BODY()

public:
	AFriendlyAi();

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UWidgetComponent* WidgetComp;

	UPROPERTY()
	UEmojiComponent* EmojiComp;

	virtual void TaskCompleted() override;

	virtual void TaskStarted() override;
};
