// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SBlackholeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API ASBlackholeProjectile : public ASProjectileBase
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
public:	
	/* Sets default values for this actor's properties */
	ASBlackholeProjectile();

	/** Add a custom behavior on LifeSpanExpired */
	virtual void LifeSpanExpired() override;

	
protected:
	/** Used to implements physics to pull objects. Necessary...I mean, it's a black-hole */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Physics)
	TObjectPtr<URadialForceComponent> RadialForceComponent;

	/** Sphere component for inhale (suck and destroy object) logic */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Physics)
	TObjectPtr<USphereComponent> SphereInhaleCollision;
	
	/** Float curve that defines the relative scale on every second of remaining lifespan */
	UPROPERTY(EditDefaultsOnly, Category=Physics)
	TObjectPtr<UCurveFloat> RelativeScaleCurve;

	/** Value for lifespan */
	UPROPERTY(EditDefaultsOnly, Category=Time)
	float LifeTime;

	/** Array with the Trapped Actors by the Black-hole projectile */
	TArray<TObjectPtr<AActor>> TrappedActors;
	
	/** Array with the actors that were trapped but eventually get
	 *  out from the collision black-hole and pending to restore scale */
	TArray<TObjectPtr<AActor>> ReleasedActors;

	
/*********************************** METHODS *********************************/
protected:
	/** Internal function between Constructor and BeginPlay */
	virtual void PostInitializeComponents() override;

	/** Logic on all frames */
	virtual void Tick(float DeltaSeconds) override;

	/** OnBeginOverlap for SphereInhaleCollision for inhale (suck and destroy object) logic */
	UFUNCTION()
	void OnInhaleSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** Shrink effect for trapped actors
	 * @fixme Works properly only with meshes with default relative scale at 1 */
	void ShrinkTrappedActors(const float DeltaSeconds);

	/** De-shrink: restore original scale
	 * @fixme Works properly only with meshes with default relative scale at 1 */
	void DeshrinkReleasedActors(const float DeltaSeconds);

	/** Get value of every curve moment */
	float GetRelativeScaleCurveValue(const float Seconds) const;

	
/************************************ DEBUG **********************************/
};
