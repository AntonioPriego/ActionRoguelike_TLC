// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"

#include "SAttributesComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{	
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);

	// Set up some specific values for MagicProjectile
	Damage = 20.0f;
	MovementComponent->InitialSpeed = 7000.0f;
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
	if (OtherActor  &&  OtherActor!=GetInstigator())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 10.0f, 12, FColor::Red, false, 1.0f);
		UActorComponent* Component = OtherActor->GetComponentByClass(USAttributesComponent::StaticClass());

		if (USAttributesComponent* AttributeComp = Cast<USAttributesComponent>(Component))
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -Damage);
		}
		
		Explode();
	}

}