#include "AI/WarriorAi.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainCharacter.h"
#include "Components/StatsComponent.h"

void AWarriorAi::AttackResult()
{
	if (MainChar && !MainChar->IsHasShield() && IsCharacterInAttackRadius() && GetDistanceTo(MainChar) < AttackDistance + AttackRange)
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
