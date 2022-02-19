#include "UI/Magic/BookOfMagic.h"
#include "Components/MagicComponent.h"
#include "Components/ScrollBox.h"
#include "Components/Overlay.h"
#include "Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void UBookOfMagic::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (auto* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
	{
		MagicComp = MainChar->GetMagicComponent();
		CreateSpells();
		MagicComp->OnSpellsCountChanged.AddDynamic(this, &UBookOfMagic::CreateSpells);
	}
}

void UBookOfMagic::NativeDestruct()
{
	Super::NativeDestruct(); 
	if (auto* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		MagicComp = MainChar->GetMagicComponent();
		MagicComp->OnSpellsCountChanged.RemoveAll(this);
	}
}

void UBookOfMagic::CreateSpells()
{
	if (SpellsContainer && MagicComp)
	{
		SpellsContainer->ClearChildren();
		for (const auto& Spell: MagicComp->GetActiveSpells())
		{
			SpellsContainer->AddChild(UBookElement::Create(Spell, this, ElementWidgetClass));
		}
	}
}

void UBookOfMagic::OnSpellSelected(const FSpell& SelectedSpell)
{
	if (PropertyContainer)
	{
		PropertyContainer->ClearChildren();
		PropertyContainer->AddChildToOverlay(UBookSpellStat::Create(GetWorld(), SelectedSpell, StatWidgetClass));
	}
}