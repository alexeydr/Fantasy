#include "AI/FriendlyAi.h"
#include "AI/FriendlyAIController.h"
#include "EngineUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Texture2D.h"
#include "Components/WidgetComponent.h"
#include "Components/EmojiComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/DialogComponent.h"

AFriendlyAi::AFriendlyAi()
{
	PrimaryActorTick.bCanEverTick = true;
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	EmojiComp = CreateDefaultSubobject<UEmojiComponent>("EmojiComponent");
	DialogComp = CreateDefaultSubobject<UDialogComponent>("DialogComponent");
	
}

void AFriendlyAi::BeginPlay()
{
	Super::BeginPlay();
}

void AFriendlyAi::TaskCompleted()
{
	if (RandomParticleComp)
	{
		RandomParticleComp->DestroyComponent();
	}
	EmojiComp->HideEmoji();
	Super::TaskCompleted();
}

void AFriendlyAi::TaskStarted()
{
	if (CurrentTask.bSpawnRandomParticle && CurrentTask.AllRandomParticlesDT)
	{
		TArray<FRandomParticles*> DTElements;
		FString ContextString;
		TArray<FName> RowNames;
		RowNames = CurrentTask.AllRandomParticlesDT->GetRowNames();
		for (auto& name : RowNames)
		{
			FRandomParticles* row = CurrentTask.AllRandomParticlesDT->FindRow<FRandomParticles>(name, ContextString);
			if (row)
			{
				DTElements.Add(row);
			}
		}
		if (DTElements.Num() > 0)
		{
			if (RandomParticleComp)
			{
				RandomParticleComp->DestroyComponent();
			}
			auto* RandomElementFromDT = DTElements[FMath::RandRange(0, DTElements.Num() - 1)];
			RandomParticleComp = UGameplayStatics::SpawnEmitterAtLocation(this, RandomElementFromDT->EmitterTemplate, GetActorLocation(), FRotator(), RandomElementFromDT->EmitterScale);
		}
	}

	if (CurrentTask.bNeedEmoji)
	{
		EmojiComp->WatchEmoji();
		EmojiComp->SetEmojiWidget(CurrentTask.EmojiTexture);
	}
	Super::TaskStarted();
}
