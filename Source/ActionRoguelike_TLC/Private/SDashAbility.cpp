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

	MovementComponent->InitialSpeed = 10000.0f;

	// Attach OnComponentHit method
	SphereComponent->OnComponentHit.AddDynamic(this, &ASDashAbility::OnHit);
}


// Called when the game starts or when spawned
void ASDashAbility::BeginPlay()
{
	Super::BeginPlay();

	ParticleEnterTeleportComponent->Activate();
	GetWorldTimerManager().SetTimer(TimerHandle_DashLifeSpan, this, &ASDashAbility::AbilityAction, 0.6f);
}


// Called every frame
void ASDashAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called on SphereComponent hit
void ASDashAbility::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                      FVector NormalImpulse, const FHitResult& Hit)
{
	AbilityAction();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("Dash SphereComponent Hitted"));
}


// Do what the Ability have to do (Dash)
void ASDashAbility::AbilityAction()
{
	GetWorldTimerManager().SetTimer(TimerHandle_WaitExplosionAnim, this, &ASDashAbility::Teleport, 0.2f);

	
	SphereComponent->OnComponentHit.Clear();
	MovementComponent->Deactivate();
	
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::White,TEXT("ASDashAbility::AbilityAction()"));
}


// Teleport instigator to this actor location
void ASDashAbility::Teleport() const
{
	const FVector  Location = GetActorLocation();
	const FRotator Rotation = FRotator(GetInstigator()->GetActorRotation().Pitch, GetActorRotation().Yaw, GetInstigator()->GetActorRotation().Roll);
	
	GetInstigator()->SetActorLocationAndRotation(Location, Rotation,false,
								  nullptr, ETeleportType::TeleportPhysics);

	ParticleExitTeleportComponent->Activate();
	EffectComponent->Deactivate();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::White,TEXT("ASDashAbility::Teleport()"));
}
