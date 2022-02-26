#include "Character/CharacterItem.h"
#include "Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ACharacterItem::ACharacterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ActorTransform.SetScale3D(FVector(1,1,1));
}

void ACharacterItem::OnInteraction_Implementation()
{
	auto* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MainChar)
	{
		MainChar->EquipNewItem(this);
		EquipedEffect();
		FTimerHandle TH;
		GetWorld()->GetTimerManager().SetTimer(TH, this, &ACharacterItem::LastingEffect, LastingEffectTime, true);
	}
}

void ACharacterItem::BeginDestroy()
{
	UnEquipedEffect();
	Super::BeginDestroy();
}

FText USalableItem::GetSalableItemName_Implementation()
{
	return ItemInfo.Name;
}

FText USalableItem::GetSalableItemDesc_Implementation()
{
	return ItemInfo.Description;
}

UTexture2D* USalableItem::GetSalableItemTexture_Implementation()
{
	return ItemInfo.Texture;
}

float USalableItem::GetSalableItemPrice_Implementation()
{
	return ItemInfo.Price;
}

void USalableItem::OnItemSaled_Implementation(UObject* WCO)
{
	if (WCO)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto* Item = WCO->GetWorld()->SpawnActor<ACharacterItem>(ItemInfo.ItemClass, SpawnInfo);
		if (Item)
		{
			Item->OnInteraction();
		}
	}
}

bool USalableItem::CheckAlreadyBought_Implementation(UObject* WCO)
{
	if (auto* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(WCO, 0)))
	{
		if (!MainChar->GetAttachedActor() && !ItemInfo.ItemClass.Get())
			return false;

		return (MainChar->GetAttachedActor()->GetClass()->GetName() == ItemInfo.ItemClass.Get()->GetName());
		
	}
	return true;
}