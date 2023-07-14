// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASProjectileBase::ASProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	// Set DestroyActorOnExplode true on default
	DestroyActorOnExplode = true;

	// Set up and take SphereComponent as RootComponent
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->SetSphereRadius(6.0f);
	RootComponent = SphereComponent;

	ImpactVfx = CreateDefaultSubobject<UParticleSystem>("ImpactVfx");

	// Basic set up for EffectComponent and attach to SphereComponent
	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(SphereComponent);

	// Set up MovementComponent and some initial values
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed                 = 1000.0f;
	MovementComponent->bRotationFollowsVelocity     = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	Damage = 0.0f;
}


// Internal function between Constructor and BeginPlay
void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Attach OnComponentHit method
	SphereComponent->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}


void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}


// _Implementation from it being marked as BlueprintNativeEvent 
void ASProjectileBase::Explode_Implementation()
{
	if (ensure(IsValid(this))) {
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation());

		if (DestroyActorOnExplode)
			Destroy();
	}
}


// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

