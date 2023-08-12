// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "SPickUpItem.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API ASPickUpItem : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	ASPickUpItem();
	

protected:
	/* When interface function declared as BlueprintNativeEvent on UFUNCTION
    we have to add "_Implementation", bc we are using it on C++ but on BLUEPRINTS too */
	// Definition of Interact function of SGameplayInterface on PickUpItem
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	// Set Actor enabled or disabled: visibility and collisions
	void SetActiveStatus(const bool Active);
	void Enable();
	void Disable();
	

	/** Mesh Component for PickUpItem */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	/** When true: respawn after RespawnSeconds. When false: Destroy on use */
	UPROPERTY(VisibleAnywhere, Category=Values)
	bool IsReSpawnable;

	/** Time to respawn the item if IsReSpawnable */
	UPROPERTY(VisibleAnywhere, Category=Values)
	float RespawnSeconds;


private:
	//Timers
	FTimerHandle TimerHandle_Respawn;
};
