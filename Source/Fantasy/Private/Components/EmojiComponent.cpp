#include "Components/EmojiComponent.h"
#include "UI/Emoji/EmojiWidget.h"
#include "Engine/Texture2D.h"
#include "Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/DialogComponent.h"

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
		{
			SetEmojiWidget(nullptr);
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}

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

void UEmojiComponent::OnDialogUpdated(EDialogStatus Status, AActor* Bot)
{
	if (Bot == GetOwner())
	{
		switch (Status)
		{
		case EDialogStatus::Started:
			WatchEmoji();
			break;

		case EDialogStatus::TalkReplic:
			if (RandomEmoji.Num() > 0)
				SetEmojiWidget(RandomEmoji[FMath::RandRange(0, RandomEmoji.Num() - 1)]);
			break;

		case EDialogStatus::ReplicTalked:
			SetEmojiWidget(nullptr);
			break;

		case EDialogStatus::Completed:
			HideEmoji();
			break;
		}
	}
}

void UEmojiComponent::BeginPlay()
{
	Super::BeginPlay();
	MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	WidgetComp = GetOwner()->FindComponentByClass<UWidgetComponent>();
	auto* DialogSubsys = GetOwner()->GetGameInstance()->GetSubsystem<UDialogSubsystem>();
	auto* DialogComp = GetOwner()->FindComponentByClass<UDialogComponent>();
	if (DialogComp && DialogSubsys && DialogComp->bCanStartRandomDialog)
	{
		DialogSubsys->OnDialogUpdateState.AddDynamic(this, &ThisClass::OnDialogUpdated);
	}
	
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

