// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include "SAttributesComponent.h"


// Make a damage amount to an actor
bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	if (DamageCauser && TargetActor)
	{
		USAttributesComponent* AttrComponent = USAttributesComponent::GetAttributes(TargetActor);
		if (AttrComponent)
		{
			return AttrComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
		}
	}

	return false;
}


// ApplyDamage but with directional impulse on hit
bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent  &&  HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComponent->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}

		return true;
	}

	return false;
}
