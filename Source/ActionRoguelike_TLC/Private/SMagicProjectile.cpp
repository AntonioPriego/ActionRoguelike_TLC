// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"

#include "SAction.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionEffect.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{	
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);

	// Set up some specific values for MagicProjectile
	Damage = 20.0f;
	MovementComponent->InitialSpeed = 7000.0f;

	// LifeSpan
	InitialLifeSpan = 8.0f;
}


// Internal function between Constructor and BeginPlay
void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Add OnBeginOverlap to SphereComponent
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}


// _Implementation from it being marked as BlueprintNativeEvent 
void ASMagicProjectile::Explode_Implementation()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactShake, GetActorLocation(), 650, 1800);

	DrawDebugSphere(GetWorld(),GetActorLocation(),650,32, FColor::Green,false,5);
	DrawDebugSphere(GetWorld(),GetActorLocation(),1800,32, FColor::Emerald,false,5);
	
	Super::Explode_Implementation();
}


// Called when actor overlaps with something
void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						               bool bFromSweep, const FHitResult& SweepResult)
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying");
	
	if (OtherActor  &&  OtherActor!=GetInstigator())
	{
		USActionComponent* ActionComponent = OtherActor->FindComponentByClass<USActionComponent>();
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComponent->Velocity = -MovementComponent->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));

			return;
		}
		
	 	DrawDebugSphere(GetWorld(), GetActorLocation(), 10.0f, 12, FColor::Red, false, 1.0f);

		if ( USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult) )
		{
			Explode();

			if (ActionComponent)
			{
				ActionComponent->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}