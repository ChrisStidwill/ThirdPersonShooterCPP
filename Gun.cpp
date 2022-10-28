// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "DrawDebugHelpers.h"


// Sets default values
AGun::AGun()
{
 	//Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AGun::PullTrigger()
{
	//particles and sound
	if (HitParticles) UGameplayStatics::SpawnEmitterAttached(HitParticles, Mesh, TEXT("MuzzleFlashSocket"));
	if (FireSound) UGameplayStatics::SpawnSoundAttached(FireSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	FRotator Rotation;
	bool bSuccess = GunTrace(Hit, ShotDirection, Rotation);
	if (bSuccess)
	{
		if (ImpactSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		}
		if (ImpactParticles) 
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, Hit.Location - Rotation.Vector()*10, ShotDirection.Rotation());
		}
		FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
		if (!Hit.GetActor())
		{
			return;
		}
		AController* OwnerController = GetOwnerController();
		Hit.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult &Hit, FVector& ShotDirection, FRotator& Rotation)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;
	FVector Location;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();
	FVector End = Location + Rotation.Vector() * MaxRange; 	//Gets vector in direction of your rotation.
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner()); // Collisions ignore gun & actor
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}