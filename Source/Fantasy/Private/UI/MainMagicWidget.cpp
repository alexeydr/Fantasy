#include "UI/MainMagicWidget.h"
#include "Components/MagicComponent.h"
#include "Components/HorizontalBox.h"
#include "UI/Magic/SpellWidget.h"

void UMainMagicWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (MainChar)
	{
		MagicComp = MainChar->GetMagicComponent();
		if (MagicComp)
		{
			UpdateMana(MagicComp->GetMana());

			MagicComp->OnManaValueChanged.AddDynamic(this, &ThisClass::UpdateMana);

			MagicComp->OnSpellsCountChanged.AddDynamic(this, &ThisClass::CreateActiveSpells);
			CreateActiveSpells();
		}
	}
	

}

void UMainMagicWidget::NativeDestruct()
{
	if (MainChar && MagicComp)
	{
		MagicComp->OnManaValueChanged.RemoveAll(this);
	
	}
	Super::NativeDestruct();
}

void UMainMagicWidget::CreateActiveSpells()
{
	if (!MagicComp || !SpellWidgetClass || !SpellsBox)
		return;

	SpellsBox->ClearChildren();

	for (const auto& Spell: MagicComp->GetActiveSpells())
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
