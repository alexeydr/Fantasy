#include "Character/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/MagicComponent.h"
#include "Camera/CameraComponent.h"
#include "SpellBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StatsComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Interfaces/InteractionInterface.h"

AMainCharacter::AMainCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;	
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	MagicComp = CreateDefaultSubobject<UMagicComponent>(TEXT("Magic"));
	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats"));
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interact"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	PrimaryActorTick.bCanEverTick = true;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("CastSpell1", IE_Pressed, this, &AMainCharacter::CastSpellTemplate<1>);
	PlayerInputComponent->BindAction("CastSpell2", IE_Pressed, this, &AMainCharacter::CastSpellTemplate<2>);
	PlayerInputComponent->BindAction("CastSpell3", IE_Pressed, this, &AMainCharacter::CastSpellTemplate<3>);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AMainCharacter::OnCharacterInteraction);
}

void AMainCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::CastSpell(int Number)
{
	if (MagicComp)
	{
		MagicComp->PrepareForCastSpell(Number);
	}
}

void AMainCharacter::OnCharacterInteraction()
{
	if (InteractionActor && InteractionActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		IInteractionInterface::Execute_OnInteraction(InteractionActor);
}

bool AMainCharacter::IsHasShield()
{
	TArray<AActor*> Spells;
	UGameplayStatics::GetAllActorsOfClass(this, ASpellBase::StaticClass(), Spells);
	for (auto* Spell : Spells)
	{
		auto* MagicSpell = Cast<ASpellBase>(Spell);
		if (MagicSpell && MagicSpell->SpellStruct.TypeSpell == ESpellType::Shield)
		{
			return true;
		}
	}
	return false;
}