// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

	
/********************************* PROPERTIES ********************************/
protected:
	/** To set projectile from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<AActor> ProjectileClass;
	
	/** To set attack anim from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	TObjectPtr<UAnimMontage> AttackAnim;
	
	/** Manage the max distance to check hit actor on projectile trajectory */
	UPROPERTY(EditAnywhere, Category=Attack)
	float HitAttackRange;

	/** Manage the max distance to check hit actor on projectile trajectory */
	UPROPERTY(VisibleAnywhere, Category=Attack)
	FName HandSocketName;

	/** Delay between projectile action is input and projectile action is casted */
	UPROPERTY(VisibleAnywhere, Category=Attack)
	float AttackAnimDelay;

	// Timers
	FTimerHandle TimerHandle_ProjectileDelay;
	FTimerDelegate Delegate;
	
/*********************************** METHODS *********************************/
public:
	/** Sets default values for this character's properties */
	USAction_ProjectileAttack();
	
	/** Defines behavior at action start */
	virtual void StartAction_Implementation(AActor* Instigator) override;

	/** Defines behavior at action end */
	virtual void StopAction_Implementation(AActor* Instigator) override;


protected:
	/** Called when TimerHandle_ProjectileDelay time is elapsed */
	UFUNCTION(BlueprintCallable)
	void ProjectileDelay_TimeElapsed(AActor* InstigatorActor);

	/** Called to spawn projectiles from player */
	void SpawnProjectile(AActor* Instigator);
};
