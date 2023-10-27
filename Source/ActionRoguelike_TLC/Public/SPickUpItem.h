// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickUpItem.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API ASPickUpItem : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
	/** Mesh Component for PickUpItem */
	UPROPERTY(ReplicatedUsing="OnRep_CoinPickedUp", VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	/** When true: respawn after RespawnSeconds. When false: Destroy on use */
	UPROPERTY(EditAnywhere, Category=Respawn)
	bool IsReSpawnable;

	/** Time to respawn the item if IsReSpawnable */
	UPROPERTY(EditAnywhere, Category=Respawn)
	float RespawnSeconds;

	/** Price for picking up this item */
	UPROPERTY(VisibleAnywhere, Category=Credits)
	int32 CreditsCost;

	/** With this set to true, we ensure the player press the interaction button to pick it up */
	UPROPERTY(EditAnywhere, Category=Credits)
	bool SecurePickUp;

	
/*********************************** METHODS *********************************/
public:	
	// Sets default values for this actor's properties
	ASPickUpItem();
	

protected:
	/* When interface function declared as BlueprintNativeEvent on UFUNCTION
    we have to add "_Implementation", bc we are using it on C++ but on BLUEPRINTS too */
	/** Definition of Interact function of SGameplayInterface on PickUpItem */
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	/** @return True when behaviour needs object pickup. If false, player does not need object pickup
	 * The actual behavior of the pick up item */
	virtual bool OnPickUpBehavior(APawn* InstigatorPawn);

	/** When object is picked up logic */
	void PickUp();

	/** Replicated pick up */
	UFUNCTION()
	void OnRep_CoinPickedUp();

	/** Set Actor enabled or disabled: visibility and collisions */
	void SetActiveStatus(const bool Active);
	void Enable();
	void Disable();
	

private:
	//Timers
	FTimerHandle TimerHandle_Respawn;
};
