// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpItem.h"


// Sets default values
ASPickUpItem::ASPickUpItem()
{
	// Set up MeshComponent default, as a root and collision profile
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName("PickUp");

	// Set up some values
	RespawnSeconds    = 10.0f;
	IsReSpawnable     = true;
}


// Definition of Interact function of SGameplayInterface on PickUpItem
// On parent only logic to manage respawn or destroy (generic logic)
void ASPickUpItem::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);


	if (IsReSpawnable)
	{
		// Disabled and reEnabled in RespawnSeconds seconds
		Disable();		
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPickUpItem::Enable, RespawnSeconds);
	}
	else
	{
		// Clear timer due to possible inconsistencies and destroy bc is not respawnable
		GetWorldTimerManager().ClearTimer(TimerHandle_Respawn);		
		Destroy();
	}
}


// Set enable or disable
void ASPickUpItem::SetActiveStatus(const bool Active)
{
	// Clear timer due to possible inconsistencies
	GetWorldTimerManager().ClearTimer(TimerHandle_Respawn);
	
	// Visibility
	MeshComponent->SetVisibility(Active);

	// Collisions
	SetActorEnableCollision(Active);
}

// Unreal C++ standards recommends avoid using inline, so I just leave it at one line to earn space and that's all
void ASPickUpItem::Enable()  { SetActiveStatus(true ); }
void ASPickUpItem::Disable() { SetActiveStatus(false); }
