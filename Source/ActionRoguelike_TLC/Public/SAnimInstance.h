// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class USActionComponent;


UCLASS()
class ACTIONROGUELIKE_TLC_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

/********************************* PROPERTIES ********************************/
protected:
	/** */
	UPROPERTY(BlueprintReadOnly, Category=Animation)
	USActionComponent* ActionComponent;
	
	/** Is Pawn Stunned based on GameplayTag data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	bool bIsStunned;

	
/*********************************** METHODS *********************************/
protected:
	/** ENGINE: Native initialization override point */
	void NativeInitializeAnimation() override;

	/** ENGINE: Native update override point. It is usually a good idea to simply gather data in this step and
	 *  for the bulk of the work to be done in NativeThreadSafeUpdateAnimation */
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
