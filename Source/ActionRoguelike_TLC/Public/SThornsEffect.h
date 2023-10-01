// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SThornsEffect.generated.h"

class USAttributesComponent;


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API USThornsEffect : public USActionEffect
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
	UPROPERTY(EditDefaultsOnly, Category=Thorns)
	float DamageBackRate;
	
/*********************************** METHODS *********************************/
public:
	/** Sets default values for this component's properties */
	USThornsEffect();
	
	/** Defines behavior at action effect start */
	virtual void StartAction_Implementation(AActor* Instigator) override;

	/** Defines behavior at action effect end */
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	/** Send back DamageBackRate of the damage caused */
	UFUNCTION()
	void ApplyThorns(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	/** Get casted owner attributes component */
	USAttributesComponent* GetOwnerAttComponent() const;
};
