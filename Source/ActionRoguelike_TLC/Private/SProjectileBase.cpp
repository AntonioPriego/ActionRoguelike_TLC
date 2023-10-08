// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "SCharacter.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASProjectileBase::ASProjectileBase()
{
	// Set DestroyActorOnExplode true on default
	DestroyActorOnExplode = true;

	// Set up and take SphereComponent as RootComponent
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->SetSphereRadius(6.0f);
	RootComponent = SphereComponent;
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);

	// Basic set up for EffectComponent and attach to SphereComponent
	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(RootComponent);
	
	// Set point name for SKMesh player on projectile casting location
	CastAttachPointName = "Muzzle_01";
	
	
	// Set up audio
	FlightLoopAudioComponent = CreateDefaultSubobject<UAudioComponent>("FlightLoopAudioComponent");
	FlightLoopAudioComponent->SetupAttachment(RootComponent);

	// Set up MovementComponent and some initial values
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed                 = 4000.0f;
	MovementComponent->bRotationFollowsVelocity     = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	Damage = 0.0f;

	// Server replication
	bReplicates = true;
}


// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Spawn sound attached to the MagicProjectile itself
	if (FlightLoopSound)
	{
		FlightLoopAudioComponent = UGameplayStatics::SpawnSoundAttached(FlightLoopSound, SphereComponent);
	}

	// Spawn Particle effect attached to SCharacter (PlayerCharacter_C) hand ("Muzzle_01")
	if (GetInstigator()  &&  GetInstigator()->GetClass()->GetFName() == FName("PlayerCharacter_C"))
	{
		if (USkeletalMeshComponent* SkeletalMesh = Cast<ASCharacter>(GetInstigator())->GetSKMesh())
		{
			if (CastVfx  &&  SkeletalMesh)
			{
				UGameplayStatics::SpawnEmitterAttached(CastVfx, SkeletalMesh, CastAttachPointName);
			}
		}
	}
}


// Internal function between Constructor and BeginPlay
void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Attach OnComponentHit method
	SphereComponent->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}


// 'virtual' so we can override ths in child-classes
void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}


// _Implementation from it being marked as BlueprintNativeEvent 
void ASProjectileBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{

		// Visuals and sound on impact
		if (ImpactVfx)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation());
		}
		
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
		}
		
		if (FlightLoopAudioComponent)
		{
			FlightLoopAudioComponent->Stop();
		}
		
		if (DestroyActorOnExplode)
		{
			Destroy();
		}
	}
}
