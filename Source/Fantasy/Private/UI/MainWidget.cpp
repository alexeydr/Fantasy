#include "UI/MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StatsComponent.h"

void UMainWidget::OnDestroyedPlayer(AActor* DestroyedActor)
{
	RemoveFromParent();
}

void UMainWidget::OnHealthChanged(float NewHealth)
{
	if (HealthText)
	{
		HealthText->SetText(FText::AsNumber(static_cast<int>(NewHealth)));
	}
}

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MainChar)
	{
		MainChar->OnDestroyed.AddDynamic(this, &ThisClass::OnDestroyedPlayer);
		if (auto* StatsComp = MainChar->GetStatsComponent())
		{
			OnHealthChanged(StatsComp->GetHealth());
			UpdateMoney(StatsComp->GetMoney());

			StatsComp->OnMoneyChanged.AddDynamic(this, &ThisClass::UpdateMoney);
			StatsComp->OnHealthChangedDelegate.AddDynamic(this, &ThisClass::OnHealthChanged);
		}
	}
}

void UMainWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (MainChar)
	{
		MainChar->OnDestroyed.RemoveAll(this);
		if (auto* StatsComp = MainChar->GetStatsComponent())
		{
			StatsComp->OnHealthChangedDelegate.RemoveAll(this);
		}
	}
}

void UMainWidget::UpdateMoney(float NewMoneyValue)
{
	if (MoneyText)
	{
		MoneyText->SetText(FText::AsNumber(static_cast<int>(NewMoneyValue)));
	}
}