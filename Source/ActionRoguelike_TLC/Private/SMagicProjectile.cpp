// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"

#include "SAttributesComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
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
	Super::Explode_Implementation();
}


// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called when actor overlaps with something
void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						               bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor  &&  OtherActor!=GetInstigator()) {
		USAttributesComponent* AttributeComp = Cast<USAttributesComponent>( OtherActor->GetComponentByClass(USAttributesComponent::StaticClass()) );

		if (AttributeComp) {
			AttributeComp->ApplyHealthChange(-20.0f);

			Explode();
		}
	}
}