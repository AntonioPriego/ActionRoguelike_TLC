// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpItem.h"
#include "SCoinPickUp.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API ASCoinPickUp : public ASPickUpItem
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
	UPROPERTY(VisibleAnywhere, Category=Credits)
	int32 CreditsEarned;

	/** Only for anim purposes, simply adds variation to anim*/
	float Adder;
	
/*********************************** METHODS *********************************/
protected:
	/** Sets default values */
	ASCoinPickUp();
	
	/** The actual behavior of the pick up item */
	virtual bool OnPickUpBehavior(APawn* InstigatorPawn) override;

	/** Called every frame */
	virtual void Tick(float DeltaSeconds) override;
};
