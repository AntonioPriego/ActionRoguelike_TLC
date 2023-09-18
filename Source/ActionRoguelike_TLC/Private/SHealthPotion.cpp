// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributesComponent.h"
#include "Components/AudioComponent.h"


// Sets default values
ASHealthPotion::ASHealthPotion()
{
	// Set up some values
	DeltaHealthChange = 20.0f;
	RespawnSeconds    = 10.0f;
	CreditsCost       = 20   ;
	IsReSpawnable     = true;
}


// The actual behavior of the pick up item
// RETURN true if needed object to be picked up
bool ASHealthPotion::OnPickUpBehavior(APawn* InstigatorPawn)
{
	// Error management
	if (!ensure(InstigatorPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("InstigatorPawn not found on SHealthPotion Interact"));
		return false;
	}

	// Cast and apply healthChange if correct
	USAttributesComponent* AttributesComponent = InstigatorPawn->FindComponentByClass<USAttributesComponent>();
	if (ensure(AttributesComponent))
	{
		// Ignores when Health is Full
		if (!AttributesComponent->IsFullHealth())
		{
			AttributesComponent->ApplyHealthChange(this, DeltaHealthChange);
			
			return true;
		}
	}

	return false;
}