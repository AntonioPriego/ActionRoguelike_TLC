// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashAbility.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASDashAbility::ASDashAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set DestroyActorOnExplode to false bc we want the parent Explode don't destroy the actor
	//to destroy it when ParticleExitTeleport is finished 
	DestroyActorOnExplode = false;

	// Basic set up for visuals components and attach to SphereComponent
	ParticleEnterTeleportComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleEnterTeleportComponent");
	ParticleEnterTeleportComponent->SetupAttachment(SphereComponent);
	ParticleEnterTeleportComponent->SetAutoActivate(false);
	ParticleExitTeleportComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleExitTeleportComponent");
	ParticleExitTeleportComponent->SetupAttachment(SphereComponent);
	ParticleExitTeleportComponent->SetAutoActivate(false);

	// Ignore Player when moving
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);

	// Set up some class values
	MovementComponent->InitialSpeed = 3500.0f;
	TeleportDelay   = 0.35f;
	DetonationDelay = 0.2f;
	InitialLifeSpan = 2.0f;
}


// Called when the game starts or when spawned
void ASDashAbility::BeginPlay()
{
	Super::BeginPlay();

	ParticleEnterTeleportComponent->Activate();
	GetWorldTimerManager().SetTimer(TimerHandle_DashLifeSpan, this, &ASDashAbility::AbilityAction, TeleportDelay);
}


// Called every frame
void ASDashAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// _Implementation from it being marked as BlueprintNativeEvent 
void ASDashAbility::Explode_Implementation()
{
	// Call parent Explode() but DestroyActorOnExplode is false, so it wont destroy actor. So we must deactivate EffectComponent
	Super::Explode_Implementation();
	EffectComponent->Deactivate();
	
	GetWorldTimerManager().ClearTimer(TimerHandle_DashLifeSpan); // Clear timer bc we are calling AbilityAction before expected for timer
	
	AbilityAction();
}


// Do what the Ability have to do (Dash)
void ASDashAbility::AbilityAction()
{
	GetWorldTimerManager().SetTimer(TimerHandle_WaitExplosionAnim, this, &ASDashAbility::Teleport, DetonationDelay);

	// Deactivate movement and collisions bc the active interaction of the actor with the world is finished at this time
	MovementComponent->Deactivate();
	SetActorEnableCollision(false);
}


// Teleport instigator to this actor location
void ASDashAbility::Teleport()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ActorToTeleport) {
		// Keep instigator rotation or it may end up jarring
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}

	// Some visuals
	ParticleExitTeleportComponent->Activate();
	EffectComponent->Deactivate();
	
	
	//Destroy();
}

