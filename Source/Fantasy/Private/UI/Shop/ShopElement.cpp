// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopElement.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Character/MainCharacter.h"
#include "Components/MagicComponent.h"
#include "Components/StatsComponent.h"
#include "Kismet/GameplayStatics.h"

void UShopElement::NativeConstruct()
{
	auto* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (BuyItem && MainChar)
	{
		StatsComp = MainChar->GetStatsComponent();
		if (StatsComp)
		{
			StatsComp->OnMoneyChanged.AddDynamic(this, &ThisClass::OnMoneyChanged);
		}
		BuyItem->OnClicked.AddDynamic(this, &ThisClass::OnBuyItemClicked);
	}
}

void UShopElement::OnMoneyChanged(float NewMoney)
{
	if (NewMoney < ItemPrice && BuyItem)
	{
		BuyItem->SetIsEnabled(false);
	}
}

void UShopElement::OnBuyItemClicked()
{
	if (StatsComp && StatsComp->CheckCanBuyItem(ItemPrice))
	{
		SetAlreadyBoughtStatus();
		ISaleableInterface::Execute_OnItemSaled(Item.ItemInterface.GetObject(), this);
		StatsComp->SubstractMoney(ItemPrice);
	}
}

void UShopElement::SetAlreadyBoughtStatus()
{
	if (AlreadyBought && BuyItem)
	{
		BuyItem->SetIsEnabled(false);
		AlreadyBought->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UShopElement::CreateSaleableItem(const FSalebleItems& InItem)
{
	if (!ItemImage || !ItemName || !ItemDesc || !ItemPriceTB)
		return;
	
	Item = InItem;
	ItemPrice = ISaleableInterface::Execute_GetSalableItemPrice(Item.ItemInterface.GetObject());
	ItemImage->SetBrushFromTexture(ISaleableInterface::Execute_GetSalableItemTexture(Item.ItemInterface.GetObject()));
	ItemName->SetText(ISaleableInterface::Execute_GetSalableItemName(Item.ItemInterface.GetObject()));
	ItemDesc->SetText(ISaleableInterface::Execute_GetSalableItemDesc(Item.ItemInterface.GetObject()));
	ItemPriceTB->SetText(FText::AsNumber(ItemPrice));

	if (ISaleableInterface::Execute_CheckAlreadyBought(Item.ItemInterface.GetObject(), this))
	{
		SetAlreadyBoughtStatus();
	}

}