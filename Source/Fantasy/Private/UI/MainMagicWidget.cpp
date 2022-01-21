#include "UI/MainMagicWidget.h"
#include "Components/MagicComponent.h"
#include "Components/HorizontalBox.h"
#include "UI/Magic/SpellWidget.h"

void UMainMagicWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (MainChar)
	{
		if (auto* MagicComp = MainChar->GetMagicComponent())
		{
			UpdateMana(MagicComp->GetMana());

			MagicComp->OnManaValueChanged.AddDynamic(this, &ThisClass::UpdateMana);

			CreateActiveSpells(MagicComp->GetActiveSpells());
		}
	}
	

}

void UMainMagicWidget::NativeDestruct()
{
	if (MainChar)
	{
		if (auto* MagicComp = MainChar->GetMagicComponent())
		{
			MagicComp->OnManaValueChanged.RemoveAll(this);
		}
	}
	Super::NativeDestruct();
}

void UMainMagicWidget::CreateActiveSpells(const TArray<FSpell>& Spells)
{
	if (!SpellWidgetClass || !SpellsBox)
		return;

	for (const auto& Spell: Spells)
	{
		auto* Widget = CreateWidget<USpellWidget>(GetWorld(), SpellWidgetClass);
		if (Widget)
		{
			Widget->InitializeWidget(Spell);
			SpellsBox->AddChild(Widget);
		}
	}
}

void UMainMagicWidget::UpdateMana(float NewManaValue)
{
	if (ManaText)
	{
		ManaText->SetText(FText::AsNumber(static_cast<int>(NewManaValue)));
	}
}
