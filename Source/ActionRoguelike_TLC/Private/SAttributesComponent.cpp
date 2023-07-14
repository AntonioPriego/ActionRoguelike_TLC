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


// Apply the Delta increment/decrement to Health and return true if it was successful
bool USAttributesComponent::ApplyHealthChange(float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);;
	

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	

	return true;
}


// Return if Health is greater than 0
bool USAttributesComponent::IsAlive() const
{
	return Health > 0.0f;
}