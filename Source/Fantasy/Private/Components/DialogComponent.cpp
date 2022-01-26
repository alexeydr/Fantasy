#include "Components/DialogComponent.h"
#include "AI/BaseAi.h"
#include "Engine/EngineTypes.h"

// Sets default values for this component's properties
UDialogComponent::UDialogComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDialogComponent::BeginPlay()
{
	Super::BeginPlay();

	auto* DialogSubsys = GetOwner()->GetGameInstance()->GetSubsystem<UDialogSubsystem>();
	if (DialogSubsys && bCanStartRandomDialog)
	{
		DialogSubsys->OnDialogUpdateState.AddDynamic(this, &ThisClass::OnDialogUpdated);
	}

}


// Called every frame
void UDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDialogComponent::OnDialogUpdated(EDialogStatus Status, AActor* Bot)
{
	if (Bot == GetOwner())
	{
		FTimerHandle TH;
		switch (Status)
		{
		case EDialogStatus::TalkReplic:
			GetWorld()->GetTimerManager().SetTimer(TH, this, &UDialogComponent::TalkNewReplic, ReplicLifeTime, false);
			break;
		case EDialogStatus::Completed:
			if (auto* Ai = Cast<ABaseAi>(GetOwner()))
				Ai->ContinueCurrentTask();

			break;
		}
	}
}

void UDialogComponent::TalkNewReplic()
{
	auto* DialogSubsys = GetOwner()->GetGameInstance()->GetSubsystem<UDialogSubsystem>();
	if (DialogSubsys)
	{
		DialogSubsys->TalkNextReplic(GetOwner());
	}
}
