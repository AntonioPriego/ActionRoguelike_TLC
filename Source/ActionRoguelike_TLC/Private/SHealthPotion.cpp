// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributesComponent.h"


// Sets default values
ASHealthPotion::ASHealthPotion()
{
	// Set up some values
	DeltaHealthChange = 20.0f;
	RespawnSeconds    = 10.0f;
	IsReSpawnable     = true;
}


// Definition of Interact function of SGameplayInterface on PickUpItem
void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	// Error management
	if (!ensure(InstigatorPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("InstigatorPawn not found on SHealthPotion Interact"));
		return;
	}
	
	
	// Get Attribute Component
	UActorComponent*       ActorComponent      = InstigatorPawn->GetComponentByClass(USAttributesComponent::StaticClass());
	USAttributesComponent* AttributesComponent = Cast<USAttributesComponent>(ActorComponent);

	// Cast and apply healthChange if correct
	if (ensure(ActorComponent))
	{
		// Ignores when Health is Full
		if (!AttributesComponent->IsFullHealth())
		{
			AttributesComponent->ApplyHealthChange(this, DeltaHealthChange);

			Super::Interact_Implementation(InstigatorPawn); // Manages respawn or destroy logic
		}
	}
}
