// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


// Starts this task, should return Succeeded, Failed or InProgress
EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	const AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		// GET SHOOT ROTATION
		// So we need to get de vector between THIS actor muzzle and TargetActor:
		// -Muzzle location                         |-> Controller->Pawn->PawnMesh->Muzzle location (Muzzle_01)
		// -TargetActor location                    |
		// -Rotation between muzzle and target      |-> Target location - Muzzle location
		const ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (!MyPawn)
		{
			return EBTNodeResult::Failed;
		}
		const FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (!TargetActor)
		{
			return EBTNodeResult::Failed;
		}

		const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		const FRotator Rotation = Direction.Rotation();

		// SPAWN PROJECTILE
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		const AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, Rotation, Params);

		
		return (NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
	}
	
	return EBTNodeResult::Failed;
}