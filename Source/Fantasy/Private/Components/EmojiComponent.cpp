#include "Components/EmojiComponent.h"
#include "UI/Emoji/EmojiWidget.h"
#include "Engine/Texture2D.h"
#include "Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

UEmojiComponent::UEmojiComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEmojiComponent::WatchEmoji()
{
	if (WidgetComp)
	{
		// made to play widget animation
		if (auto* Widget = WidgetComp->GetWidget())
			Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible); 

		WidgetComp->SetVisibility(true);
	}
}

void UEmojiComponent::HideEmoji()
{
	if (WidgetComp)
	{
		if (auto* Widget = WidgetComp->GetWidget())
			Widget->SetVisibility(ESlateVisibility::Collapsed);

		WidgetComp->SetVisibility(false);
	}
}

void UEmojiComponent::SetEmojiWidget(UTexture2D* Emoji)
{
	if (WidgetComp)
	{
		if (auto* Widget = Cast<UEmojiWidget>(WidgetComp->GetWidget()))
		{
			Widget->SetNewEmoji(Emoji);
		}
	}
}

void UEmojiComponent::ChangeMood(EMoodName NewMode)
{

}

void UEmojiComponent::BeginPlay()
{
	Super::BeginPlay();
	MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	WidgetComp = GetOwner()->FindComponentByClass<UWidgetComponent>();
	
}

void UEmojiComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (MainChar && WidgetComp && WidgetComp->IsVisible())
	{
		FVector Diff = MainChar->GetActorLocation() - WidgetComp->GetComponentLocation();
		FRotator NewRotation = FRotationMatrix::MakeFromX(Diff).Rotator();
		WidgetComp->SetWorldRotation(NewRotation);
	}

}

