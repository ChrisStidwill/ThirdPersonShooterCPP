// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "ShooterGameModeBase.h"
#include "ShooterPlayerController.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Refs
	PlayerControllerRef = Cast<AShooterPlayerController>(GetController());
	Health = MaxHealth;
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);

}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health/MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("ToggleCombat"), IE_Pressed, this, &AShooterCharacter::ToggleCombat);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), IE_Pressed, this, &AShooterCharacter::PullTrigger);
	
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= FMath::Min(DamageToApply, Health);

	FVector DamagerLocation = DamageCauser->GetActorLocation();
	FVector MyLocation = GetActorLocation();
	FVector MyForward = GetActorForwardVector();
	if ((MyLocation - DamagerLocation).Length() <= (MyLocation + MyForward - DamagerLocation).Length())
	{
		bShotFromFront = false;
	}
	else
	{
		bShotFromFront = true;
	}

	if (AShooterCharacter::IsDead())
	{
		AShooterCharacter::HandleDeath();
	}
	
	return DamageToApply;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::PullTrigger()
{
	if (!bInCombat || !Gun)
	{
		return;
	}
	Gun->AGun::PullTrigger();

}

void AShooterCharacter::ToggleCombat()
{
	if (bInCombat)
	{
		bInCombat = false;
		if (PlayerControllerRef != nullptr) PlayerControllerRef->SetCrosshairsHidden(true);
	}
	else
	{
		bInCombat = true;
		if (PlayerControllerRef != nullptr) PlayerControllerRef->SetCrosshairsHidden(false);
	}
}

void AShooterCharacter::SetCombatStatus(bool bSetInCombat)
{
	bInCombat = bSetInCombat;
}

void AShooterCharacter::HandleDeath()
{
	AShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AShooterGameModeBase>();
	if (GameMode != nullptr)
	{
		GameMode->PawnKilled(this);
	}
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}




