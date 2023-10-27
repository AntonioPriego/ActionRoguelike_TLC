// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"


// CVars
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);



// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	// Health default values
	MaxHealth = 100;
	Health    = MaxHealth;
	// Rage default values
	MaxRage = 100;
	Rage    = 0;
	RatioDamageRage = 0.1f;

	// Server replication 
	SetIsReplicatedByDefault(true);
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
	const float NewHealth = FMath::Clamp(Health + Delta, 0, MaxHealth);
	const float ClampedDelta = NewHealth - PreviousHealth;

	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if (ClampedDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ClampedDelta); // Server updates (so local too)
		}
		
		// Died
		if (ClampedDelta < 0.0f  &&  Health == 0)
		{
			OnOwnerKilled(InstigatorActor);
		}
	}


	if (ClampedDelta < 0.0f)
	{
		RageIncrease(-ClampedDelta);
	}

	
	return (ClampedDelta != 0);
}


// Manages owner killed calling game-mode
void USAttributesComponent::OnOwnerKilled(AActor* Killer) const
{
	ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
	if (GameMode)
	{
		GameMode->OnActorKilled(GetOwner(), Killer);
	}
}


// Add Rage amount depending on damage received
void USAttributesComponent::RageIncrease(const float DamageReceived)
{
	if (DamageReceived > 0.0f)
	{		
		const float ClampedRage  = FMath::Clamp(Rage+DamageReceived*RatioDamageRage, 0, MaxRage);
		const float ClampedDelta = ClampedRage - Rage;
		
		Rage = ClampedRage;

		OnRageChanged.Broadcast(this, Rage, -ClampedDelta);
	}
}


// Decrease Rage amount and return when DecreaseAmount is less than Rage (possible to decrease)
bool USAttributesComponent::RageDecrease(const float DecreaseAmount)
{
	if (DecreaseAmount < Rage)
	{
		const float ClampedRage  = FMath::Clamp(Rage-DecreaseAmount, 0, MaxRage);
		const float ClampedDelta = Rage - ClampedRage;
		
		Rage = ClampedRage;

		OnRageChanged.Broadcast(this, Rage, ClampedDelta);
		return true;
	}
	else if (DecreaseAmount > Rage)
	{
		return false;
	}
	
	// DecreaseAmount == 0
	return true;
}


// DEBUG: To kill the actor with this AttributesComponent during testing
void USAttributesComponent::Kill(AActor* InstigatorActor)
{
	ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}


// ENGINE: Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties
void USAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributesComponent, Health);
	DOREPLIFETIME(USAttributesComponent, MaxHealth);
	//DOREPLIFETIME_CONDITION(USAttributesComponent, MaxHealth, COND_OwnerOnly); // Optimized option
}


// Needed for HealthChanged server replication
void USAttributesComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


// Getters/Setters and simple conditions
bool  USAttributesComponent::IsAlive() const            { return Health > 0.0f;       }
bool  USAttributesComponent::IsFullHealth() const       { return Health == MaxHealth; }
float USAttributesComponent::GetHealth() const          { return Health;              }
float USAttributesComponent::GetMaxHealth() const       { return MaxHealth;           }
float USAttributesComponent::GetRage() const            { return Rage;                }
float USAttributesComponent::GetMaxRage() const         { return MaxRage;             }
float USAttributesComponent::GetRatioDamageRage() const { return RatioDamageRage;     }
void  USAttributesComponent::SetRatioDamageRage(const float NewRatio) { RatioDamageRage = NewRatio; }

