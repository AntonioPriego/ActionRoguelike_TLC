// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SExplosiveBarrel.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<URadialForceComponent> RadialForceComponent;

	
/*********************************** METHODS *********************************/
public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();
	
	
protected:
	// Internal function between Constructor and BeginPlay
	virtual void PostInitializeComponents() override;

	// Called when onComponentHit triggers
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse, const FHitResult& Hit);
};
