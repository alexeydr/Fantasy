// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "DialogSubsystem.h"
#include "EmojiComponent.generated.h"

class UEmojiWidget;
class UWidgetComponent;
class UTexture2D;
class AMainCharacter;

UENUM(BlueprintType)
enum class EMoodName : uint8
{
	Test1     UMETA(DisplayName = "Test1"),
	Test2      UMETA(DisplayName = "Test2")

};

USTRUCT(BlueprintType)
struct FMoodState : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoodName MoodName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> MoodTextures;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASY_API UEmojiComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEmojiComponent();

	void WatchEmoji();

	void HideEmoji();

	void SetEmojiWidget(UTexture2D* Emoji);

	void ChangeMood(EMoodName NewMode);

protected:

	UFUNCTION()
	void OnDialogUpdated(EDialogStatus Status, AActor* Bot);

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> RandomEmoji;

	UPROPERTY(EditDefaultsOnly, meta = (RequiredAssetDataTags = "RowStructure=MoodState"))
	UDataTable* AvailableMoods;

	UPROPERTY()
	UWidgetComponent* WidgetComp;

	UPROPERTY()
	AMainCharacter* MainChar;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
