// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SAttributesComponent.h"
#include "SBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API USBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	USBTService_CheckLowHealth();
	
protected:
	/** update next tick interval
	 * this function should be considered as const (don't modify state of object) if node is not instanced!
	 * bNotifyTick must be set to true for this function to be called 
	 * Calling INIT_SERVICE_NODE_NOTIFY_FLAGS in the constructor of the service will set this flag automatically */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Get if AttributesComponent have low Health
	bool IsLowHealth(const USAttributesComponent* AttributesComponent) const;

	/** Select the key to set low health status */
	UPROPERTY(EditAnywhere, Category=AI)
	FBlackboardKeySelector LowHealthKey;

	/** Property to provide the limit percentage to determine that lower values are 'Low health' */
	UPROPERTY(EditAnywhere, Category=AI, meta=(ClampMin="0", ClampMax="1"))
	float LowHealthPercent;
};
