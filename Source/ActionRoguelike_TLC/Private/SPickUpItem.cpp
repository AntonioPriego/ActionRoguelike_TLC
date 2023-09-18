// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpItem.h"
#include "SCharacter.h"


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
	CreditsCost       = 0; // Default value is free! :O
}


// Definition of Interact function of SGameplayInterface on PickUpItem
// On parent only logic to manage respawn or destroy (generic logic)
void ASPickUpItem::Interact_Implementation(APawn* InstigatorPawn)
{
	// Get CreditsComponent and check if it is enough to pick up this item
	USCreditsComponent* CreditsComponent = InstigatorPawn->FindComponentByClass<USCreditsComponent>();
	if (CreditsComponent)
	{
		if (CreditsComponent->HaveEnoughCredits(CreditsCost))
		{
			if (OnPickUpBehavior(InstigatorPawn))
			{
				CreditsComponent->RemoveCredits(CreditsCost);
				PickUp();
			}
		}
	}
}


// The actual behavior of the pick up item
bool ASPickUpItem::OnPickUpBehavior(APawn* InstigatorPawn) { return true; }


// When object is picked up logic
void ASPickUpItem::PickUp()
{
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
