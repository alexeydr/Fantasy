#include "DialogSubsystem.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "AI/BaseAi.h"
#include "Components/DialogComponent.h"

void UDialogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FTimerHandle TH;
	GetWorld()->GetTimerManager().SetTimer(TH, this, &UDialogSubsystem::StartDialogs, 35.f, true);
	Super::Initialize(Collection);
}

void UDialogSubsystem::Deinitialize()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::Deinitialize();
}

void UDialogSubsystem::ShuffleArray(TArray<AActor*>& Arr)
{
	if (Arr.Num() > 0)
	{
		int32 LastIndex = Arr.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Arr.Swap(i, Index);
			}
		}
	}
}

void UDialogSubsystem::StartDialogs()
{
	TArray<AActor*> AllBots;
	UGameplayStatics::GetAllActorsOfClass(this, ABaseAi::StaticClass(), AllBots);
	ShuffleArray(AllBots);
	for (auto* BotA : AllBots)
	{
		if (CurrentRuntimeDialogs >= MaxRuntimeDialogs)
			return;

		auto* Base = Cast<ABaseAi>(BotA);
		if (!BotA->FindComponentByClass<UDialogComponent>() || HasCooldown(BotA) || BotsInDialog.Contains(BotA) || Base->bTaskStarted)
			continue;

		for (auto* BotB : AllBots)
		{
			Base = Cast<ABaseAi>(BotB);
			if (!BotB->FindComponentByClass<UDialogComponent>() || BotsInDialog.Contains(BotB) || HasCooldown(BotB) || Base->bTaskStarted)
				continue;

			if (BotA != BotB &&	BotA->GetDistanceTo(BotB) < 1000.f)
			{
				// TODO: Refactor
				BotsInDialog.Add(BotA);
				BotsInDialog.Add(BotB);
				auto* DialogBotA = Cast<ABaseAi>(BotA);
				DialogBotA->MoveTarget = BotB;
				auto* DialogBotB = Cast<ABaseAi>(BotB);
				DialogBotB->MoveTarget = BotA;
				PairsInDialog.Add(FDialogPair(BotA, BotB));
				CurrentRuntimeDialogs++;
				break;
			}
		}
	}
}

void UDialogSubsystem::StartDialog(const FDialogPair& Pair)
{
	if (OnDialogUpdateState.IsBound())
	{
		OnDialogUpdateState.Broadcast(EDialogStatus::Started, Pair.BotA);
		OnDialogUpdateState.Broadcast(EDialogStatus::Started, Pair.BotB);

		Cast<ABaseAi>(Pair.BotA)->MoveTarget = nullptr;
		Cast<ABaseAi>(Pair.BotB)->MoveTarget = nullptr;

		OnDialogUpdateState.Broadcast(EDialogStatus::TalkReplic, FMath::RandBool() ? Pair.BotA : Pair.BotB);
	}		
	
}

void UDialogSubsystem::TalkNextReplic(AActor* Talking)
{
	FDialogPair DeleteElement;
	bool bNeedDelete = false;
	for (auto& Pair : PairsInDialog)
	{
		if (Pair.BotA == Talking || Pair.BotB == Talking)
		{
			if (OnDialogUpdateState.IsBound())
			{
				OnDialogUpdateState.Broadcast(EDialogStatus::ReplicTalked, Talking);
				if (Pair.NumReplics < MaxReplics)
				{
					OnDialogUpdateState.Broadcast(EDialogStatus::TalkReplic, Pair.BotA == Talking ? Pair.BotB : Pair.BotA);
					Pair.NumReplics++;
					break;
				}
				else
				{
					bNeedDelete = true;
					DeleteElement = Pair;
					break;
				}
			}

		}
	}
	if (bNeedDelete)
	{
		PairsInDialog.Remove(DeleteElement);
		EndDialog(DeleteElement);
	}

}

void UDialogSubsystem::EndDialog(const FDialogPair& Pair)
{
	if (OnDialogUpdateState.IsBound())
	{
		OnDialogUpdateState.Broadcast(EDialogStatus::Completed, Pair.BotA);
		OnDialogUpdateState.Broadcast(EDialogStatus::Completed, Pair.BotB);
	}

	CompleteDialog(Pair.BotA);
	CompleteDialog(Pair.BotB);
}

void UDialogSubsystem::CompleteDialog(AActor* Bot)
{
	float CurrentTime = UGameplayStatics::GetTimeSeconds(this);

	if (DialogColdownBots.Contains(Bot))
		DialogColdownBots[Bot] = CurrentTime;
	else
		DialogColdownBots.Add(Bot, CurrentTime);

	BotsInDialog.Remove(Bot);

	CurrentRuntimeDialogs--;
}

bool UDialogSubsystem::HasCooldown(AActor* Actor)
{
	float* Value = DialogColdownBots.Find(Actor);
	
	float CurrentTime = UGameplayStatics::GetTimeSeconds(this);

	if (!Value || *Value + ColdownTime < CurrentTime)
	{
		return false;
	}
	
	return true;
}

void UDialogSubsystem::Tick(float DeltaTime)
{
	for (auto& Pair: PairsInDialog)
	{
		if (!Pair.bDialogStarted && Pair.BotA->GetDistanceTo(Pair.BotB) < 100.f)
		{
			Pair.bDialogStarted = true;
			StartDialog(Pair);
		}
	}
}

TStatId UDialogSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDialogSubsystem, STATGROUP_Tickables);
}
