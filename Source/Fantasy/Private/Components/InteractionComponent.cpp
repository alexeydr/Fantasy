#include "Components/InteractionComponent.h"
#include "Character/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/InteractionInterface.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	MainChar = Cast<AMainCharacter>(GetOwner());
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!InteractionWidgetClass || !MainChar || !MainChar->GetCameraComponent())
		return;

	FHitResult HitResult;
	FVector Start = MainChar->GetCameraComponent()->GetComponentLocation();
	FVector ForwardVector = MainChar->GetCameraComponent()->GetForwardVector();
	FVector End = Start + ForwardVector * 1000.f;

	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(MainChar);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParameters);
	bool bCanInteract = UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UInteractionInterface::StaticClass());

	if (bCanInteract)
	{
		if (!InteractionWidgetPtr)
		{
			InteractionWidgetPtr = CreateWidget<UUserWidget>(GetWorld(), InteractionWidgetClass);
			if (InteractionWidgetPtr)
			{
				InteractionWidgetPtr->AddToViewport();
			}
		}
		MainChar->SetInteractionActor(HitResult.GetActor());
	}
	else
	{
		if (InteractionWidgetPtr)
		{
			InteractionWidgetPtr->RemoveFromParent();
			InteractionWidgetPtr = nullptr;
		}
		MainChar->SetInteractionActor(nullptr);
	}

}

