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

	if (!CheckCanLife())
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

bool UStatsComponent::CheckCanLife()
{
	return !FMath::IsNearlyZero(Health);
}

void UStatsComponent::Dying()
{

	if (OnDeathDelegate.IsBound())
		OnDeathDelegate.Broadcast();

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		OwnerActor->Destroy();
	}
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

