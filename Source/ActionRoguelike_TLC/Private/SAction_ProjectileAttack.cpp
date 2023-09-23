// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "SCharacter.h"


// Sets default values for this character's properties
USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	// Some constructor values
	HitAttackRange  = 3700.0f;
	AttackAnimDelay = 0.12f;							
	HandSocketName  = "Muzzle_01";
}


// Defines behavior at action start
void USAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	

	ASCharacter* Character = Cast<ASCharacter>(InstigatorActor);
	const bool IsAttackTimerPending = GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_ProjectileDelay);
	if (Character && !IsAttackTimerPending)
	{
		Character->PlayAnimMontage(AttackAnim);
		
		Delegate.BindUFunction(this, "ProjectileDelay_TimeElapsed", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ProjectileDelay, Delegate, AttackAnimDelay, false);
	}
}


// Defines behavior at action end
void USAction_ProjectileAttack::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
}


// Called when TimerHandle_PrimaryAttack time is elapsed
void USAction_ProjectileAttack::ProjectileDelay_TimeElapsed(AActor* InstigatorActor)
{
	SpawnProjectile(InstigatorActor);

	StopAction(InstigatorActor);
}


// Called to spawn projectiles from player
void USAction_ProjectileAttack::SpawnProjectile(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ASCharacter* Instigator = Cast<ASCharacter>(InstigatorActor);
	if (Instigator)
	{
		// Getting some parameters for sweep raycast projectile trajectory searching hit points 
		const FVector HandLocation = Instigator->GetMesh()->GetSocketLocation(HandSocketName);
		FHitResult HitResult;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		// To ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Instigator);	

		// Calculation of the projectile trajectory and possible hit point
		const FVector SweepStartPoint = Instigator->GetPawnViewLocation();
		const FVector SweepEndPoint   = Instigator->GetPawnViewLocation() + Instigator->GetPawnViewForwardVector()*HitAttackRange;
		const bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult, SweepStartPoint, SweepEndPoint, ObjectQueryParams, Params);
		
		// Calculation of the projectile spawn rotation from the hand (HandLocation to ProjectileEndLocation)
		const FVector ProjectileEndLocation = bBlockingHit ? HitResult.Location : SweepEndPoint;
		const FRotator ProjectileRotation = (ProjectileEndLocation - HandLocation).Rotation();

		// Setting some parameters for projectile spawn
		const FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Instigator;

		// Projectile Spawn
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		// DEBUG: Draw sphere on ProjectileEndLocation
		DrawDebugSphere(GetWorld(),ProjectileEndLocation,32.0f,32.f,FColor::Black,false,3.0f);
	}
}