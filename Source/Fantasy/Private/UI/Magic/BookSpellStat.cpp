#include "UI/Magic/BookSpellStat.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UBookSpellStat* UBookSpellStat::Create(UWorld* World, const FSpell& Spell, TSubclassOf<UBookSpellStat> WidgetClass)
{
	auto* CreatedWidget = CreateWidget<UBookSpellStat>(World, WidgetClass);
	if (CreatedWidget)
	{
		CreatedWidget->SetInfo(Spell);
	}
	return CreatedWidget;
}

void UBookSpellStat::SetInfo(const FSpell& Spell)
{
	if (SpellImage && Name && Description && Damage && Type && ManaPrice)
	{
		CurrentSpell = Spell;
		SpellImage->SetBrushFromTexture(Spell.Texture);
		Name->SetText(Spell.Name);
		Description->SetText(Spell.Description);
		ManaPrice->SetText(FText::AsNumber(Spell.Mana));
		Damage->SetText(FText::AsNumber(Spell.Damage));
		Type->SetText(UEnum::GetDisplayValueAsText(Spell.TypeSpell));
	}
}