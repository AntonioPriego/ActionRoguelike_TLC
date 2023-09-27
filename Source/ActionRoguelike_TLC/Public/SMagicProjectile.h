// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraShakeBase.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;



UCLASS()
class ACTIONROGUELIKE_TLC_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
	/** Tag name for parry */
	UPROPERTY(EditAnywhere, Category=Damage)
	FGameplayTag ParryTag;

	/** Burning secondary effect on impact */
	UPROPERTY(EditDefaultsOnly, Category=Damage)
	TSubclassOf<USActionEffect> BurningActionClass;

	
/*********************************** METHODS *********************************/
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

	
protected:
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
