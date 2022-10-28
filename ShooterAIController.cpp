// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();
    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
        // if (PlayerPawn == nullptr) return;
        if (ControlledCharacter == nullptr) return;
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), ControlledCharacter->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"), ControlledCharacter->PatrolLocation);
    }

    //SetFocus makes them face the player.


}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);    
}

bool AShooterAIController::IsDead() const
{
   AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
   if (ControlledCharacter != nullptr)
   {
        return ControlledCharacter->IsDead();
   }

   return true;
}
