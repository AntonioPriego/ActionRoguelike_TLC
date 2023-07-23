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


public:
	// Sets default values
	ASHealthPotion();


protected:
	/* When interface function declared as BlueprintNativeEvent on UFUNCTION
	we have to add "_Implementation", bc we are using it on C++ but on BLUEPRINTS too */
	// Definition of Interact function of SGameplayInterface on PickUpItem
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;


	/** The health value increased on HealthPotion cure */
	UPROPERTY(VisibleAnywhere, Category=Values)
	float DeltaHealthChange;
};
