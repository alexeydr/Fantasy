#include "AI/EnemyAi.h"
#include "Components/StatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainCharacter.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "AIController.h"
#include "SpellBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Animation/AnimSequence.h"
#include "Components/SphereComponent.h"

AEnemyAi::AEnemyAi()
{
	StatsComponent = CreateDefaultSubobject<UStatsComponent>("StatsComp");
}

void AEnemyAi::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (const auto& Act: UpdatedActors)
	{
		if (Act == MainChar)
		{
			GoToAttackPosition();
		}
	}
}

void AEnemyAi::ChangeFightModeStatus(bool bNewStatus)
{
	if (bFightModeActive == bNewStatus)
		return;

	bFightModeActive = bNewStatus;
}

void AEnemyAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bool bCanFight = MoveTarget && GetDistanceTo(MoveTarget) <= MoveTargetDistance;
	ChangeFightModeStatus(bCanFight);
	if (bCanFight)
	{
		Attack();
	}

	if (GetMesh() && GetMesh()->GetAnimationMode() == EAnimationMode::AnimationSingleNode && !GetMesh()->IsPlaying())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); 
		if (MovementComp)
		{
			MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		bCanMove = true;

		switch (CurrentState)
		{
		case EAiState::Attack:
			AttackResult();
			break;
		}
		CurrentState = EAiState::None;
	}

}

void AEnemyAi::OnDeath()
{

}

void AEnemyAi::OnCharacterCastSpell(ESpellStatus Status, const FSpell& Spell)
{
	if (bFightModeActive && MoveTarget && Status == ESpellStatus::StartedCast)
	{
		if (GetDistanceTo(MoveTarget) <= MoveTargetDistance && Spell.TypeSpell != ESpellType::Shield)
		{
			Attack();
			return;
		}
	}
}

void AEnemyAi::BeginPlay()
{
	Super::BeginPlay();
	MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PerceptionComp = FindComponentByClass<UAIPerceptionComponent>();
	SphereComp = FindComponentByClass<USphereComponent>();
	MovementComp = FindComponentByClass<UCharacterMovementComponent>();
	if (SphereComp && PerceptionComp && MainChar && StatsComponent)
	{
		UMagicComponent* MagicComp = MainChar->GetMagicComponent();
		if (MagicComp)
		{
			MagicComp->OnSpellsStatusChanged.AddDynamic(this, &ThisClass::OnCharacterCastSpell);
		}
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCompBeginOverlap);
		StatsComponent->OnDeathDelegate.AddDynamic(this, &ThisClass::OnDeath);
		PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
	}
}

void AEnemyAi::OnCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* Spell = Cast<ASpellBase>(OtherActor);
	if (MoveTarget && GetDistanceTo(MoveTarget) > 300.f && Spell && Spell->SpellStruct.TypeSpell == ESpellType::Projectile)
	{
		FHitResult HitResult;
		FVector Start = Spell->GetActorLocation();
		FVector ForwardVector = Spell->GetActorForwardVector();
		FVector End = Start + ForwardVector * 5000.f;

		FCollisionQueryParams CollisionParameters;
		TArray<AActor*> IgnoredActors;
		for (TActorIterator<AActor> ItActor = TActorIterator<AActor>(GetWorld(), AActor::StaticClass()); ItActor; ++ItActor)
		{
			if (Cast<AEnemyAi>(*ItActor))
				continue;

			IgnoredActors.Add(*ItActor);
		}
		CollisionParameters.AddIgnoredActors(IgnoredActors);

		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParameters);
		bool bHitProjectile = (HitResult.GetActor() == this);

		if (bHitProjectile)
		{
			DodgeMagic();
		}

	}
}

////////////////////// ANIMATIONS

void AEnemyAi::DodgeMagic()
{
	if (CurrentState != EAiState::None)
		return;

	CurrentState = EAiState::Block;
	if (MovementComp)
	{
		MovementComp->DisableMovement();
	}
	GetMesh()->PlayAnimation(BlockAsset, false);
}

void AEnemyAi::OnDidDamage(float InDamage)
{
	DoDamage(InDamage);

	if (CurrentState != EAiState::None)
		return;

	CurrentState = EAiState::Hit;
	if (MovementComp)
	{
		MovementComp->DisableMovement();
	}
	GetMesh()->PlayAnimation(HitAsset, false);
	GoToAttackPosition();
}

void AEnemyAi::Attack()
{
	if (CurrentState != EAiState::None || !bCanAttack)
		return;

	bCanMove = false;
	CurrentState = EAiState::Attack;
	if (MovementComp)
	{
		MovementComp->DisableMovement();
	}
	GetMesh()->PlayAnimation(AttackAsset, false);
}
