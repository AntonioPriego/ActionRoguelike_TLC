// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraShakeBase.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Internal function between Constructor and BeginPlay
	virtual void PostInitializeComponents() override;

	// _Implementation from it being marked as BlueprintNativeEvent 
	UFUNCTION()
	virtual void Explode_Implementation() override;
	
	// Called when actor overlaps with something
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		                bool bFromSweep, const FHitResult& SweepResult);

};
