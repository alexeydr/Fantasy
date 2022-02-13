// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class AMainCharacter;
class UUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASY_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInteractionComponent();

protected:

	UPROPERTY()
	AMainCharacter* MainChar;

	UPROPERTY()
	UUserWidget* InteractionWidgetPtr;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InteractionWidgetClass;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
