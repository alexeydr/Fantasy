#include "AI/GruntAi.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainCharacter.h"
#include "Components/StatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void AGruntAi::AttackResult()
{
	bool bIsSuccesfullAttack = bIsMainAttack
		? (GetDistanceTo(MainChar) < AttackDistance + AttackRange && IsCharacterInAttackRadius())
		: (GetDistanceTo(MainChar) < AdditionalAttackDistance);

	if (MainChar && !MainChar->IsHasShield() && bIsSuccesfullAttack)
	{
		if (auto* StatsComp = MainChar->GetStatsComponent())
		{
			StatsComp->DoDamage(bIsMainAttack ? Damage : AdditionalDamage);
		}
	}
}

void AGruntAi::GoToAttackPosition()
{
	if (MainChar)
	{
		MoveTarget = MainChar;
	}
}

UAnimationAsset* AGruntAi::SelectAttack()
{
	bIsMainAttack = FMath::RandBool();
	return bIsMainAttack ? AttackAsset : AdditionalAttackAnim;
}

void AGruntAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AiController && bFightModeActive && GetCharacterMovement())
	{
		GetCharacterMovement()->StopActiveMovement();
	}
}
