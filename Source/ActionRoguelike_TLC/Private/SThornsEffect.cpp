// Fill out your copyright notice in the Description page of Project Settings.


#include "SThornsEffect.h"
#include "SCharacter.h"


// Sets default values for this action properties
USThornsEffect::USThornsEffect()
{
	Duration = 0.0f;
	Period   = 0.0f;
	DamageBackRate = 0.25f;
}


// Defines behavior at action effect start
void USThornsEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	const USAttributesComponent* OwnerAttComponent = GetOwnerAttComponent();
	if (OwnerAttComponent)
	{
		GetOwnerAttComponent()->OnHealthChanged.AddDynamic(this, &USThornsEffect::ApplyThorns);
	}
}


// Defines behavior at action effect end
void USThornsEffect::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	
	const USAttributesComponent* OwnerAttComponent = GetOwnerAttComponent();
	if (OwnerAttComponent)
	{
		GetOwnerAttComponent()->OnHealthChanged.RemoveDynamic(this, &USThornsEffect::ApplyThorns);
	}
}


// Send back DamageBackRate of the damage caused
void USThornsEffect::ApplyThorns(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{	
	ASCharacter* EffectOwner = Cast<ASCharacter>(GetOwningComponent()->GetOwner());
	if (EffectOwner && InstigatorActor != EffectOwner && Delta < 0.0f)
	{
		USAttributesComponent* EnemyAttributesComponent = InstigatorActor->FindComponentByClass<USAttributesComponent>();
		if (EnemyAttributesComponent && EffectOwner)
		{
			const int ThornsDamageBack = FMath::RoundToInt(Delta*DamageBackRate);
			if (ThornsDamageBack != 0)
			{
				EnemyAttributesComponent->ApplyHealthChange(EffectOwner, ThornsDamageBack);
			}
		}
	}
}


// Get casted owner attributes component
USAttributesComponent* USThornsEffect::GetOwnerAttComponent() const
{
	const AActor* EffectOwner = GetOwningComponent()->GetOwner();
	return EffectOwner->FindComponentByClass<USAttributesComponent>();
}
