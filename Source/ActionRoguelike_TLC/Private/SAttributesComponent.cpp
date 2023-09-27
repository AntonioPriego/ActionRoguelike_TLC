// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"
#include "SGameModeBase.h"


// CVars
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);



// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
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
bool USAttributesComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	// Check if (unreal internal) pawn could be damaged
	// bCanBeDamaged is a internal Pawn variable for testing modes like god-mode and stuff like that
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	const float PreviousHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);;	

	const float ClampedDelta = Health - PreviousHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ClampedDelta);

	// Died
	if (ClampedDelta < 0.0f  &&  Health == 0)
	{
		OnOwnerKilled(InstigatorActor);
	}
	
	return (ClampedDelta != 0);
}


// Getters and simple conditions
bool  USAttributesComponent::IsAlive() const      { return Health > 0.0f;       }
bool  USAttributesComponent::IsFullHealth() const { return Health == MaxHealth; }
float USAttributesComponent::GetHealth() const    { return Health;              }
float USAttributesComponent::GetMaxHealth() const { return MaxHealth;           }


// Manages owner killed calling game-mode
void USAttributesComponent::OnOwnerKilled(AActor* Killer) const
{
	ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
	if (GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("NOMBRE: %s"), *GetOwner()->GetName());
		GameMode->OnActorKilled(GetOwner(), Killer);
	}
}


// DEBUG: To kill the actor with this AttributesComponent during testing
void USAttributesComponent::Kill(AActor* InstigatorActor)
{
	ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}
