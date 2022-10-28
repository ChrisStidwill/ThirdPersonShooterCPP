// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ToggleCombat.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UBTTask_ToggleCombat : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ToggleCombat();

	// Setting Toggle Combat overrides combat setting.
	UPROPERTY(EditAnywhere)
	bool bToggleCombat = true;

	UPROPERTY(EditAnywhere)
	bool bSetCombat = true;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

};
