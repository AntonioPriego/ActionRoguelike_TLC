// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"



// Sets default values for this actor's properties
ASBlackholeProjectile::ASBlackholeProjectile()
{
	// Instantiate
	RadialForceComponent  = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	SphereInhaleCollision = CreateDefaultSubobject<USphereComponent>("SphereInhaleCollision");
	RadialForceComponent->SetupAttachment(SphereComponent);
	SphereInhaleCollision->SetupAttachment(SphereComponent);

	// Set some default values
	SphereComponent->SetSphereRadius(15.f, true);
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->InitialSpeed = 3000.0f;
	LifeTime = 5.0f;
}


// Add a custom behavior on LifeSpanExpired
void ASBlackholeProjectile::LifeSpanExpired()
{
	for (AActor* TrappedActor : TrappedActors)
	{
		TrappedActor->Destroy();
	}
	
	Super::LifeSpanExpired();
}


// Internal function between Constructor and BeginPlay
void ASBlackholeProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Link OnBeginOverlap event
	SphereInhaleCollision->OnComponentBeginOverlap.AddDynamic(this, &ASBlackholeProjectile::OnInhaleSphereBeginOverlap);

	// Set death for the projectile
	SetLifeSpan(LifeTime);
}


// Logic on all frames
void ASBlackholeProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Apply pull force
	RadialForceComponent->FireImpulse();
	DrawDebugSphere(GetWorld(), RadialForceComponent->GetComponentLocation(), 32.0f, 32, FColor::Red, false, 0.1f);

	// Shrink effect for trapped actors and De-shrink (restore scale) for released ones
	ShrinkTrappedActors(DeltaSeconds);
	DeshrinkReleasedActors(DeltaSeconds);
}


// OnBeginOverlap for SphereInhaleCollision for inhale (suck and destroy object) logic
void ASBlackholeProjectile::OnInhaleSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UStaticMeshComponent* Comp = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (Comp  &&  Comp->IsSimulatingPhysics())
		{			
			FBodyInstance* BodyInst = Comp->GetBodyInstance();
			if (BodyInst)
			{
				// Restart physics state and reduce mass scale for better inhale effect
				BodyInst->SetPhysicsDisabled(true);
				//BodyInst->SetMassScale(0.3);
				BodyInst->SetPhysicsDisabled(false);

				const int BeforeAddUniqueSize = TrappedActors.Num();
				TrappedActors.AddUnique(OtherActor);
				ReleasedActors.Remove(OtherActor);

				if (TrappedActors.Num() > BeforeAddUniqueSize)
				{
					UE_LOG(LogTemp, Log, TEXT("Added actor to Blackhole (number %d)"), TrappedActors.Num());
				}
			}	
		}
	}
}


// Shrink effect for trapped actors
void ASBlackholeProjectile::ShrinkTrappedActors(const float DeltaSeconds)
{
	int PendingTrappedActors = TrappedActors.Num()-1;
	while (PendingTrappedActors >= 0)
	{
		AActor* TrappedActor = TrappedActors[PendingTrappedActors];
		
		if (IsValid(TrappedActor))
		{
			if (TrappedActor->IsOverlappingActor(this))
			{
				const float RelativeScaleToApply = GetRelativeScaleCurveValue(5-GetLifeSpan()) * (1-DeltaSeconds);
				
				TrappedActor->SetActorRelativeScale3D(FVector(RelativeScaleToApply));				
			}
			else
			{
				ReleasedActors.AddUnique(TrappedActor);
				TrappedActors.Remove(TrappedActor);
				UE_LOG(LogTemp, Log, TEXT("Removed actor to Blackhole (number %d)"), TrappedActors.Num());
			}
		}
		else
		{
			// Actor is Destroyed or pending to be killed, so must get out of our actors arrays
			TrappedActors.Remove (TrappedActor);
			ReleasedActors.Remove(TrappedActor);
			UE_LOG(LogTemp, Log, TEXT("Removed actor to Blackhole (number %d)"), TrappedActors.Num());
		}
		
		PendingTrappedActors--;
	}
}


// De-shrink: restore scale (for actors leaving the effect of black-hole)
void ASBlackholeProjectile::DeshrinkReleasedActors(const float DeltaSeconds)
{
	int PendingReleasedActors = ReleasedActors.Num()-1;
	while (PendingReleasedActors >= 0)
	{
		AActor* ReleasedActor = ReleasedActors[PendingReleasedActors];
		
		if (IsValid(ReleasedActor))
		{
			float RelativeScaleToApply = 1 / (GetRelativeScaleCurveValue(5-GetLifeSpan()) * (1-DeltaSeconds));

			if (RelativeScaleToApply > 1.0f)
			{
				RelativeScaleToApply = 1.0f;
				ReleasedActors.Remove(ReleasedActor);
				UE_LOG(LogTemp, Error, TEXT("Actor reach to its original scale"));
			}
			
			ReleasedActor->SetActorRelativeScale3D(FVector(RelativeScaleToApply));
		}
		else
		{
			// Actor is Destroyed or pending to be killed, so must get out
			ReleasedActors.Remove(ReleasedActor);
		}
		
		PendingReleasedActors--;
	}
}


// Get value of every curve moment
float ASBlackholeProjectile::GetRelativeScaleCurveValue(const float Seconds) const
{
	if (ensure(RelativeScaleCurve))
	{		
		if (Seconds > 0.0f  &&  Seconds < LifeTime)
		{
			return RelativeScaleCurve->GetFloatValue(Seconds);
		}
	
		UE_LOG(LogTemp, Error, TEXT("[ASBlackholeProjectile::GetRelativeScaleCurveValue] Seconds out of expected range"));
		
		return 0.0f;
	}

	UE_LOG(LogTemp, Error, TEXT("[ASBlackholeProjectile::GetRelativeScaleCurveValue] RelativeScaleCurve not found"));

	return  0.0f;
}
