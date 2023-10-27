// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

	
/********************************* PROPERTIES ********************************/
protected:
	/** Base mesh for chest, the bottom static part */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;
	
	/** Lid mesh for chest, the top movable part */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LidMesh;

	/** Flag to know if lid is currently open */
	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly, SaveGame) // ReplicatedUsing = RepNotify (on BP)
	bool bLidOpened;
	
	
/*********************************** METHODS *********************************/
public:	
	// Sets default values for this actor's properties
	ASItemChest();

	/** To set rotation degrees on chest open */
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	
protected:
	/** Definition of Interact function of SGameplayInterface on SItemChest */
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	/** Get the message is going to show on screen to interact */
	virtual FText GetInteractMessage_Implementation(APawn* InstigatorPawn) override;
	
	/** Make necessary logic when Actor is loaded */
	void OnActorLoaded_Implementation(APawn* InstigatorPawn);

	/** This method is auto called by unreal when our bLidOpened updates */
	UFUNCTION()
	void OnRep_LidOpened();
};
