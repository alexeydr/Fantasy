#include "UI/Magic/SpellWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USpellWidget::InitializeWidget(const FSpell& Spell)
{
	if (auto* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		MagicComp = MainChar->GetMagicComponent();
		if (MagicComp)
		{
			MagicComp->OnSpellsStatusChanged.AddDynamic(this, &ThisClass::UpdateActiveSpell);
			ActiveSpell = Spell;
			SpellImage->SetBrushFromTexture(Spell.Texture);
		}
	}
}

void USpellWidget::UpdateActiveSpell(ESpellStatus Status, const FSpell& Spell)
{
	if (Status == ESpellStatus::StartedCast)
	{
		if (ActiveSpell == Spell && InactiveTexture)
		{
			SpellImage->SetBrushFromTexture(InactiveTexture);
		}
		SetRenderOpacity(0.5f);
	}
	if (Status == ESpellStatus::CastedCompleted)
	{
		SetRenderOpacity(1.f);
	}

	if (ActiveSpell == Spell && Status == ESpellStatus::CooldownCompleted)
	{
		SpellImage->SetBrushFromTexture(Spell.Texture);
	}
}

void USpellWidget::NativeDestruct()
{
	if (MagicComp)
	{
		MagicComp->OnManaValueChanged.RemoveAll(this);
	}
	Super::NativeDestruct();
}
