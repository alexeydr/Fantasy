#include "AI/MageAi.h"
#include "Character/MainCharacter.h"
#include "Components/StatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "AIController.h"

void AMageAi::AttackResult()
{
	if (!MainChar)
		return;

	if (MagicActor && bCanAttack)
	{
		SpawnedActor = GetWorld()->SpawnActor<AActor>(MagicActor);
		if (SpawnedActor)
		{
			SpawnedActor->SetActorLocation(MainChar->GetActorLocation());
			SpawnedActor->AttachToActor(MainChar, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
			bCanAttack = false;
			SpawnIllusion();
			FTimerHandle TH;
			GetWorld()->GetTimerManager().SetTimer(TH, this, &ThisClass::OnMagicDelayCompleted, MagicDelay, false);
		}

	}
}

void AMageAi::OnMagicDelayCompleted()
{
	SetActorHiddenInGame(false);
	bCanAttack = true;
	if (MainChar && !MainChar->IsHasShield())
	{
		if (auto* StatsComp = MainChar->GetStatsComponent())
		{
			StatsComp->DoDamage(Damage);
		}
	}
}

void AMageAi::GoToAttackPosition()
{
	if (MainChar)
	{
		MoveTarget = MainChar;
	}
}

void AMageAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AiController && bFightModeActive && GetCharacterMovement())
	{
		GetCharacterMovement()->StopActiveMovement();
	}
}

void AMageAi::SpawnIllusion()
{
	if (SpawnedIllusuion)
		SpawnedIllusuion->Destroy();

	if (IllusionClass)
	{
		SpawnedIllusuion = GetWorld()->SpawnActor<AActor>(IllusionClass);
		if (SpawnedIllusuion)
		{
			SpawnedIllusuion->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());
			UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			if (NavSys)
			{
				FNavLocation ResultLocation = FNavLocation(GetActorLocation());
				bool Result = NavSys->GetRandomReachablePointInRadius(GetActorLocation(), AttackDistance, ResultLocation);
				if (Result)
				{
					TeleportTo(ResultLocation.Location, FRotator());
					SetActorHiddenInGame(true);
				}
			}
		}
	}
	
}