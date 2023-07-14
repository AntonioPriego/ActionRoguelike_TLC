// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SProjectileBase.generated.h"

UCLASS(ABSTRACT) // 'ABSTRACT' marks this class as incomplete, keeping this out of certain dropdowns windows like SpawnActor in Unreal Editor
class ACTIONROGUELIKE_TLC_API ASProjectileBase : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Internal function between Constructor and BeginPlay
	virtual void PostInitializeComponents() override;
	
	/** Sphere root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	USphereComponent* SphereComponent;

	/** Particle system for ability visuals */
	UPROPERTY(EditDefaultsOnly, Category=Particles)
	UParticleSystemComponent* EffectComponent;
	
	/** Particle system for impact visuals */
	UPROPERTY(EditDefaultsOnly, Category=Particles)
	UParticleSystem* ImpactVfx; // UParticleSystem is just an asset of particle, so !=UParticleSystemComponent

	/** Movement component for projectiles */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	UProjectileMovementComponent* MovementComponent;

	/** Movement component for projectiles */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=OnDestroy)
	bool DestroyActorOnExplode;

	/** Variable that saves the amount of Health the projectile is going to make */
	UPROPERTY(EditAnywhere, Category=Damage)
	float Damage;


	// 'virtual' so we can override ths in child-classes
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// BlueprintNativeEvent = C++ base implementation can be expanded in BPs
	// BlueprintCallable to allow child classes to trigger explosions
	// Not required for assigment, useful for expanding in BP later on
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	
};
