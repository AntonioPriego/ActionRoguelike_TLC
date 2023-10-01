// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API USActionEffect : public USAction
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
	/** How long the action effect acts */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Effect)
	float Duration;

	/** Time between 'ticks' to apply effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Effect)
	float Period;

	// Timers
	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	
/*********************************** METHODS *********************************/
public:
	USActionEffect();
	
	/** Defines behavior at action effect start */
	virtual void StartAction_Implementation(AActor* Instigator) override;

	/** Defines behavior at action effect end */
	virtual void StopAction_Implementation(AActor* Instigator) override;
	
	
protected:
	/** Action effect acts many times every Period seconds */
	UFUNCTION(BlueprintNativeEvent, Category=Effect)
	void ExecutePeriodicEffect(AActor* Instigator);
};
