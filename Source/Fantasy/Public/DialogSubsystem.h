// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogSubsystem.generated.h"


UENUM(BlueprintType)
enum class EDialogStatus : uint8
{
	Started     UMETA(DisplayName = "Started"),
	TalkReplic    UMETA(DisplayName = "TalkReplic"),
	ReplicTalked  UMETA(DisplayName = "ReplicTalked"),
	Completed      UMETA(DisplayName = "Completed")

};

USTRUCT()
struct FDialogPair
{
	GENERATED_BODY()

public:

	UPROPERTY()
	AActor* BotA;

	UPROPERTY()
	AActor* BotB;

	UPROPERTY()
	int NumReplics = 0;

	UPROPERTY()
	int bDialogStarted = false;

	FDialogPair() = default;

	FDialogPair(AActor* NewBotA, AActor* NewBotB)
		: BotA(NewBotA), BotB(NewBotB)
	{ }

	friend bool operator==(const FDialogPair& PairL, const FDialogPair& PairR)
	{
		return (PairL.BotA == PairR.BotA || PairL.BotA == PairR.BotB) && (PairL.BotB == PairR.BotA || PairL.BotB == PairR.BotB);
	}

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogUpdateState, EDialogStatus, Status, AActor*, Bot);

UCLASS()
class FANTASY_API UDialogSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void StartDialog(const FDialogPair& Pair);

	void TalkNextReplic(AActor* Talking);

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnDialogUpdateState OnDialogUpdateState;

protected:

	UPROPERTY()
	TMap<AActor*, float> DialogColdownBots;

	UPROPERTY()
	TArray<AActor*> BotsInDialog;

	UPROPERTY()
	TArray<FDialogPair> PairsInDialog;

	float ColdownTime = 30.f;
	int MaxRuntimeDialogs = 1;
	int MaxReplics = 3;
	int CurrentRuntimeDialogs = 0;

	UFUNCTION()
	void StartDialogs();

	void ShuffleArray(TArray<AActor*>& Arr);

	void EndDialog(const FDialogPair& Pair);

	void CompleteDialog(AActor* Bot);

	bool HasCooldown(AActor* Actor);

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

};
