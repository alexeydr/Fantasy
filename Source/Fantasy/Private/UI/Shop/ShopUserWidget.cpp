#include "UI/Shop/ShopUserWidget.h"
#include "Components/ScrollBox.h"
#include "UI/Shop/ShopElement.h"

void UShopUserWidget::CreateAssortment(const TArray<FSalebleItems>& Assortment)
{
	if (!ItemsContainer || !ShopElementClass)
		return;

	for (const auto& Item : Assortment)
	{
		auto* Widget = CreateWidget<UShopElement>(GetWorld(), ShopElementClass);
		if (Widget)
		{
			Widget->CreateSaleableItem(Item);
			ItemsContainer->AddChild(Widget);
		}
	}
	

}