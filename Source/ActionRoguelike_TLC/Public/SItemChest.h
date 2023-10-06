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
	UStaticMeshComponent* BaseMesh;
	
	/** Lid mesh for chest, the top movable part */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	/** Flag to know if lid is currently open */
	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly) // ReplicatedUsing = RepNotify (on BP)
	bool bLidOpened;
	
	
/*********************************** METHODS *********************************/

public:	
	// Sets default values for this actor's properties
	ASItemChest();

	/** To set rotation degrees on chest open */
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	
protected:
	// When interface function declared as BlueprintNativeEvent on UFUNCTION
	// we have to add "_Implementation", bc we are using it on C++ but on BLUEPRINTS too
	/** Definition of Interact function of SGameplayInterface on SItemChest */
	void Interact_Implementation(APawn* InstigatorPawn);

	/** This method is auto called by unreal when our bLidOpened updates */
	UFUNCTION()
	void OnRep_LidOpened();
};
