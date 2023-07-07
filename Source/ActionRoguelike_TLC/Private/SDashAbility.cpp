// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashAbility.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASDashAbility::ASDashAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Basic set up for visuals components and attach to SphereComponent
	ParticleEnterTeleportComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleEnterTeleportComponent");
	ParticleEnterTeleportComponent->SetupAttachment(SphereComponent);
	ParticleEnterTeleportComponent->SetAutoActivate(false);
	ParticleExitTeleportComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleExitTeleportComponent");
	ParticleExitTeleportComponent->SetupAttachment(SphereComponent);
	ParticleExitTeleportComponent->SetAutoActivate(false);

	MovementComponent->InitialSpeed = 3500.0f;
	TeleportDelay   = 0.35f;
	DetonationDelay = 0.2f;
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
	GetWorldTimerManager().ClearTimer(TimerHandle_DashLifeSpan); // Clear timer bc we are calling AbilityAction before expected for timer
	AbilityAction();
}


// Do what the Ability have to do (Dash)
void ASDashAbility::AbilityAction()
{
	GetWorldTimerManager().SetTimer(TimerHandle_WaitExplosionAnim, this, &ASDashAbility::Teleport, DetonationDelay);
	
	SphereComponent->OnComponentHit.Clear();
	MovementComponent->Deactivate();
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
	

	Destroy();
}
