// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpItem.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASPickUpItem::ASPickUpItem()
{
	// Set up MeshComponent default, as a root and collision profile
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName("PickUp");

	// Set up some values
	RespawnSeconds = 10.0f;
	IsReSpawnable  = true;
	CreditsCost    = 0; // Default value is free! :O

	// Server replication
	bReplicates = true;
}


// Definition of Interact function of SGameplayInterface on PickUpItem
// On parent only logic to manage respawn or destroy (generic logic)
void ASPickUpItem::Interact_Implementation(APawn* InstigatorPawn)
{
	if (IsOverlappingActor(InstigatorPawn) && bSecurePickUp) // ensure not making interaction on overlap
	{
		return;
	}
	
	// Get PlayerState and removeCredits to apply PickupBehavior
	const ASCharacter* CastedInstigator = Cast<ASCharacter>(InstigatorPawn);
	if (CastedInstigator)
	{
		ASPlayerState* InstigatorPlayerState = CastedInstigator->GetSPlayerState();
		if (InstigatorPlayerState && InstigatorPlayerState->RemoveCredits(CreditsCost))
		{
			if (OnPickUpBehavior(InstigatorPawn))
			{
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
	OnRep_CoinPickedUp();
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


// Replicated pick up
void ASPickUpItem::OnRep_CoinPickedUp()
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


//
void ASPickUpItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickUpItem, MeshComponent);
}


void ASPickUpItem::Enable()  { SetActiveStatus(true ); }
void ASPickUpItem::Disable() { SetActiveStatus(false); }
bool ASPickUpItem::HasSecurePickUp() const { return bSecurePickUp; }
