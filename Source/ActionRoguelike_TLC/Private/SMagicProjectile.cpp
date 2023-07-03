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

	// Set up and take SphereComponent as RootComponent
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionProfileName("Projectile");	
	RootComponent = SphereComponent;

	// Basic set up for EffectComponent and attach to SphereComponent
	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(SphereComponent);

	// Set up MovementComponent and some initial values
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed                 = 1000.0f;
	MovementComponent->bRotationFollowsVelocity     = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	// Add OnBeginOverlap to SphereComponent
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}


// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
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

			Destroy();
		}
	}
}