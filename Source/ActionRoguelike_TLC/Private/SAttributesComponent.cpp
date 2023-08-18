// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"


// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Health values
	MaxHealth = 100;
	Health    = MaxHealth;
}


// Static function that returns AttributesComponent from an Actor
USAttributesComponent* USAttributesComponent::GetAttributes(const AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributesComponent>(FromActor->GetComponentByClass(USAttributesComponent::StaticClass()));
	}

	return nullptr;
}


// Static function that returns if an Actor is currently Alive. If it fails, it will return also false
bool USAttributesComponent::IsActorAlive(const AActor* ActorToCheck)
{
	if (ActorToCheck)
	{
		const USAttributesComponent* AttComponent = GetAttributes(ActorToCheck);

		if (AttComponent)
		{
			return AttComponent->IsAlive();
		}
	}

	return false;
}


// Apply the Delta increment/decrement to Health and return true if it was successful
bool USAttributesComponent::ApplyHealthChange(AActor* InstigatorActor, const float Delta)
{
	// Check if (unreal internal) pawn could be damaged
	// bCanBeDamaged is a internal Pawn variable for testing modes like god-mode and stuff like that
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	const float PreviousHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);;	

	const float ClampedDelta = Health - PreviousHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ClampedDelta);

	return true;
}


// Getters and simple conditions
bool USAttributesComponent::IsAlive() const       { return Health > 0.0f;       }
bool USAttributesComponent::IsFullHealth() const  { return Health == MaxHealth; }
float USAttributesComponent::GetHealth() const    { return Health;              }
float USAttributesComponent::GetMaxHealth() const { return MaxHealth;           }


// DEBUG: To kill the actor with this AttributesComponent during testing
void USAttributesComponent::Kill(AActor* InstigatorActor)
{
	ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}
