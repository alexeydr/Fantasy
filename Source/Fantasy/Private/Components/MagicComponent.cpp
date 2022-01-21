#include "Components/MagicComponent.h"
#include "Particles/ParticleSystemComponent.h"

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
	DecreaseMana(Spell.Price);
	if (auto* SpellActor = GetWorld()->SpawnActor<AActor>(Spell.ActorToSpawn))
	{
		if (auto* ParticleSystemComp = SpellActor->FindComponentByClass<UParticleSystemComponent>())
		{
			ParticleSystemComp->SetTemplate(Spell.Particle);
			ParticleSystemComp->InitParticles();
		}
	}
}

void UMagicComponent::PrepareForCastSpell(FSpell& Spell)
{
	if (Mana >= Spell.Price && !Spell.bIsActiveCooldown)
	{
		bCanCastSpell = false;
		Spell.bIsActiveCooldown = true;
		
		int ArrayNumber = CooldownSpells.Add(&Spell);

		if (OnSpellsStatusChanged.IsBound())
			OnSpellsStatusChanged.Broadcast(ESpellStatus::StartedCast, Spell);

		CastSpell(Spell);

		FTimerDelegate TD;
		FTimerHandle TH;
		TD.BindUFunction(this, FName("OnSpellCasted"), Spell);
		GetWorld()->GetTimerManager().SetTimer(TH, TD, Spell.CastTime, false);

		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;
		TimerDel.BindUFunction(this, FName("OnSpellCooldownCompleted"), ArrayNumber);
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

void UMagicComponent::OnSpellCooldownCompleted(int Number)
{
	if (CooldownSpells.Num() > Number)
	{
		CooldownSpells[Number]->bIsActiveCooldown = false;

		if (OnSpellsStatusChanged.IsBound())
			OnSpellsStatusChanged.Broadcast(ESpellStatus::CooldownCompleted, *CooldownSpells[Number]);
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