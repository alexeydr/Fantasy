// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EmojiWidget.generated.h"

class UImage;
class UTexture2D;

UCLASS()
class FANTASY_API UEmojiWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetNewEmoji(UTexture2D* Image);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* Emoji;
	
};
