// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogSubsystem.h"
#include "DialogComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASY_API UDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogComponent();

	UPROPERTY(EditAnywhere)
	bool bCanStartRandomDialog;

	UPROPERTY(EditAnywhere)
	float ReplicLifeTime = 2.f;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDialogUpdated(EDialogStatus Status, AActor* Bot);

	UFUNCTION()
	void TalkNewReplic();


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
