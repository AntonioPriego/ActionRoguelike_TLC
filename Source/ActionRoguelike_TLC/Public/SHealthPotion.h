// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpItem.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API ASHealthPotion : public ASPickUpItem
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
	/** The health value increased on HealthPotion cure */
	UPROPERTY(VisibleAnywhere, Category=Values)
	float HealAmount;

	
/*********************************** METHODS *********************************/
public:
	// Sets default values
	ASHealthPotion();


protected:
	/** The actual behavior of the pick up item */
	virtual bool OnPickUpBehavior(APawn* InstigatorPawn) override;
};
