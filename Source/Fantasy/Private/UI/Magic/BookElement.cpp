#include "UI/Magic/BookElement.h"
#include "UI/Magic/BookOfMagic.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

UBookElement* UBookElement::Create(const FSpell& Spell, UBookOfMagic* Parent, TSubclassOf<UBookElement> WidgetClass)
{
	if (!Parent)
		return nullptr;

	auto* CreatedWidget = CreateWidget<UBookElement>(Parent->GetWorld(), WidgetClass);
	if (CreatedWidget)
	{
		CreatedWidget->ParentWidget = Parent;
		CreatedWidget->SetInfo(Spell);
	}
	return CreatedWidget;
}

void UBookElement::SetInfo(const FSpell& Spell)
{
	if (SpellImage && Name && ShowInfo)
	{
		CurrentSpell = Spell;
		SpellImage->SetBrushFromTexture(Spell.Texture);
		Name->SetText(Spell.Name);
		ShowInfo->OnClicked.AddDynamic(this, &UBookElement::OnSpellInfoSelected);
	}
}

void UBookElement::OnSpellInfoSelected()
{
	if (ParentWidget)
	{
		ParentWidget->OnSpellSelected(CurrentSpell);
	}
}