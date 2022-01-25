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

AFriendlyAi::AFriendlyAi()
{
	PrimaryActorTick.bCanEverTick = true;
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	EmojiComp = CreateDefaultSubobject<UEmojiComponent>("EmojiComponent");
}

void AFriendlyAi::TaskCompleted()
{
	EmojiComp->HideEmoji();
	Super::TaskCompleted();
}

void AFriendlyAi::TaskStarted()
{
	if (CurrentTask.bNeedEmoji)
	{
		EmojiComp->WatchEmoji();
		EmojiComp->SetEmojiWidget(CurrentTask.EmojiTexture);
	}
	Super::TaskStarted();
}