// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	

/********************************* PROPERTIES ********************************/
protected:
	/** Set the projectile to spawn on task */
	UPROPERTY(EditAnywhere, Category=AI)
	TSubclassOf<AActor> ProjectileClass;

	/** Set maximum spread for the shoots. The bigger the number, the higher the chance of failure */
	UPROPERTY(EditAnywhere, Category=AI)
	float MaxRangedAttackSpread;


/*********************************** METHODS *********************************/
public:
	/** Sets default values */
	USBTTask_RangedAttack();
	
	
protected:
	/** starts this task, should return Succeeded, Failed or InProgress
	 *  (use FinishLatentTask() when returning InProgress)
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
