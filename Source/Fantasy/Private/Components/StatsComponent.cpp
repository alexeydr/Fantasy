#include "Components/StatsComponent.h"

UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStatsComponent::DoDamage(float Damage)
{
	Health -= Damage;

	if (OnHealthChangedDelegate.IsBound())
		OnHealthChangedDelegate.Broadcast(Health);

	if (OnDamageDelegate.IsBound())
		OnDamageDelegate.Broadcast(Damage);

	if (!IsAlive())
	{
		Dying();
	}
}

void UStatsComponent::AddHealth(float Param)
{
	Health += Param;
	if (OnHealthChangedDelegate.IsBound())
		OnHealthChangedDelegate.Broadcast(Health);
}

void UStatsComponent::AddMoney(float Param)
{
	Money += Param;
	if (OnMoneyChanged.IsBound())
		OnMoneyChanged.Broadcast(Money);
}

void UStatsComponent::SubstractMoney(float Param)
{
	Money -= Param;
	if (OnMoneyChanged.IsBound())
		OnMoneyChanged.Broadcast(Money);
}

bool UStatsComponent::IsAlive()
{
	return Health > 0.f;
}

void UStatsComponent::Dying()
{
	if (OnDeathDelegate.IsBound())
		OnDeathDelegate.Broadcast();
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

