#include "Components/MagicComponent.h"
#include "Character/MainCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpellBase.h"
#include "Components/AudioComponent.h"

UMagicComponent::UMagicComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanCastSpell = true;
}

void UMagicComponent::DecreaseMana(float Value)
{
	Mana -= Value;

	if (OnManaValueChanged.IsBound())
		OnManaValueChanged.Broadcast(Mana);
}

void UMagicComponent::AddMana(float Value)
{

	Mana += Value;

	if (OnManaValueChanged.IsBound())
		OnManaValueChanged.Broadcast(Mana);
}

void UMagicComponent::BeginPlay()
{
	Super::BeginPlay();
	AddSpellsFromDT();

}

void UMagicComponent::CastSpell(const FSpell& Spell)
{
	DecreaseMana(Spell.Mana);
	if (auto* SpellActor = GetWorld()->SpawnActor<ASpellBase>(Spell.ActorToSpawn))
	{
		SpellActor->SpellStruct = Spell;

		if (auto* AudioSystemComp = SpellActor->FindComponentByClass<UAudioComponent>())
		{
			AudioSystemComp->SetSound(Spell.Sound);
			AudioSystemComp->Play();
		}
		if (auto* ParticleSystemComp = SpellActor->FindComponentByClass<UParticleSystemComponent>())
		{
			ParticleSystemComp->SetTemplate(Spell.Particle);
			ParticleSystemComp->InitParticles();
		}
	}
}

void UMagicComponent::PrepareForCastSpell(FSpell& Spell)
{
	if (Mana >= Spell.Mana && !Spell.bIsActiveCooldown)
	{
		CurrentCastSpell = Spell.CastTime;
		bCanCastSpell = false;
		Spell.bIsActiveCooldown = true;
		
		CastSpell(Spell);

		if (OnSpellsStatusChanged.IsBound())
			OnSpellsStatusChanged.Broadcast(ESpellStatus::StartedCast, Spell);

		FTimerDelegate TD;
		FTimerHandle TH;
		TD.BindUFunction(this, FName("OnSpellCasted"), Spell);
		GetWorld()->GetTimerManager().SetTimer(TH, TD, Spell.CastTime, false);

		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;
		TimerDel.BindUFunction(this, FName("OnSpellCooldownCompleted"), Spell);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, Spell.Cooldown, false);
	}
}

void UMagicComponent::PrepareForCastSpell(int SpellNumber)
{
	if (ActiveSpells.Num() >= SpellNumber && bCanCastSpell)
	{
		PrepareForCastSpell(ActiveSpells[SpellNumber - 1]);
	}
}


void UMagicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMagicComponent::OnSpellCasted(const FSpell& SpellCompletedCast)
{
	bCanCastSpell = true;

	if (OnSpellsStatusChanged.IsBound())
		OnSpellsStatusChanged.Broadcast(ESpellStatus::CastedCompleted, SpellCompletedCast);
}

void UMagicComponent::OnSpellCooldownCompleted(FSpell& Spell)
{
	if  (Spell.bIsActiveCooldown)
	{
		int Number = ActiveSpells.Find(Spell);
		if (Number >= 0)
		{
			ActiveSpells[Number].bIsActiveCooldown = false;
		}

		if (OnSpellsStatusChanged.IsBound())
			OnSpellsStatusChanged.Broadcast(ESpellStatus::CooldownCompleted, Spell);

	}
	
}


void UMagicComponent::AddSpellsFromDT()
{
	FString ContextString;
	TArray<FName> RowNames;
	RowNames = AvailableSpells->GetRowNames();

	for (auto& name : RowNames)
	{
		FSpell* row = AvailableSpells->FindRow<FSpell>(name, ContextString);
		if (row)
		{
			ActiveSpells.Add(*row);
		}
	}
}

FText USalableSpell::GetSalableItemName_Implementation()
{
	return SpellForSale.Name;
}

FText USalableSpell::GetSalableItemDesc_Implementation()
{
	return SpellForSale.Description;
}

UTexture2D* USalableSpell::GetSalableItemTexture_Implementation()
{
	return SpellForSale.Texture;
}

float USalableSpell::GetSalableItemPrice_Implementation()
{
	return SpellForSale.Price;
}

void USalableSpell::OnItemSaled_Implementation(UObject* WCO)
{
	if (auto* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(WCO, 0)))
	{
		if (auto* MagicComp = MainChar->GetMagicComponent())
		{
			MagicComp->AddNewSpell(SpellForSale);
		}
	}
}

bool USalableSpell::CheckAlreadyBought_Implementation(UObject* WCO)
{
	if (auto* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(WCO, 0)))
	{
		if (auto* MagicComp = MainChar->GetMagicComponent())
		{
			return MagicComp->GetActiveSpells().Contains(SpellForSale);
		}
	}
	return true;
}