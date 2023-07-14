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

	/* When interface function declared as BlueprintNativeEvent on UFUNCTION
    we have to add "_Implementation", bc we are using it on C++ but on BLUEPRINTS too */
	// Definition of Interact function of SGameplayInterface on PickUpItem
	void Interact_Implementation(APawn* InstigatorPawn);
	
public:	
	// Sets default values for this actor's properties
	ASPickUpItem();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/** Mesh Component for PickUpItem */
	UMeshComponent* MeshComponent;
};
