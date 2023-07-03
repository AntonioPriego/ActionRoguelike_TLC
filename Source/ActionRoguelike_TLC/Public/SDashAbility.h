// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SDashAbility.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API ASDashAbility : public ASMagicProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDashAbility();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Particle system for explosion visuals */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleEnterTeleportComponent;

	/** Particle system for explosion visuals */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleExitTeleportComponent;

private:
	// Called on SphereComponent hit
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
               FVector NormalImpulse, const FHitResult& Hit);

	// Do what the Ability have to do (Dash)
	void AbilityAction();

	// Teleport instigator to this actor location
	void Teleport() const;
	
	// Timer handle for Actor Life Span
	FTimerHandle TimerHandle_DashLifeSpan;
	// Timer handle for explosion wait anim
	FTimerHandle TimerHandle_WaitExplosionAnim;
};
