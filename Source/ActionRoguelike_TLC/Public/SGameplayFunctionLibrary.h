// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


/*********************************** METHODS *********************************/
	public:
	/** Make a damage amount to an actor */
	UFUNCTION(BlueprintCallable, Category=Gameplay)
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);
	
	/** ApplyDamage but with directional impulse on hit */
	UFUNCTION(BlueprintCallable, Category=Gameplay)
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);
};
