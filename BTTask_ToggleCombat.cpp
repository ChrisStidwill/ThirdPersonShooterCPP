// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ToggleCombat.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ShooterCharacter.h"


UBTTask_ToggleCombat::UBTTask_ToggleCombat()
{
    NodeName = TEXT("Toggle Combat");
}

EBTNodeResult::Type UBTTask_ToggleCombat::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

    AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (Character == nullptr) return EBTNodeResult::Failed;

    if (bToggleCombat)
    {
        Character->ToggleCombat();    
    }
    else
    {
        Character->SetCombatStatus(bSetCombat);
    }

    return EBTNodeResult::Succeeded;
}
