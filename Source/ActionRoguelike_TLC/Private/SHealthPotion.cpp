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
	// Get Attribute Component
	UActorComponent* ActorComponent = InstigatorPawn->GetComponentByClass(USAttributesComponent::StaticClass());

	// Cast and apply healthChange if correct
	if (USAttributesComponent* AttributesComponent = Cast<USAttributesComponent>(ActorComponent)) {

		// Ignores when Health is Full
		if (!AttributesComponent->IsFullHealth()) {
			AttributesComponent->ApplyHealthChange(DeltaHealthChange);

			Super::Interact_Implementation(InstigatorPawn);
		}
	}
}
