#include "AI/WarriorAi.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainCharacter.h"
#include "Components/StatsComponent.h"

void AWarriorAi::AttackResult()
{
	if (MainChar && !MainChar->IsHasShield() && GetDistanceTo(MainChar) < 200.f)
	{
		if (auto* StatsComp = MainChar->GetStatsComponent())
		{
			StatsComp->DoDamage(Damage);
		}
	}

}

void AWarriorAi::GoToAttackPosition()
{
	if (MainChar)
	{
		MoveTarget = MainChar;
	}
}

void AWarriorAi::DoDamage(float InDamage)
{
	
}
