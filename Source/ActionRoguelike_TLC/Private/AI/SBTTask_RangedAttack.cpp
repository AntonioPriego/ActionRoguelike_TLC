// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "SAttributesComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


// Sets default values
USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	MaxRangedAttackSpread = 1.85f;
}

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

		// FAILED checks
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (!MyPawn)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Fail"));
			return EBTNodeResult::Failed;
		}
		
		const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (!TargetActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Failed casting TargetActor"));
			return EBTNodeResult::Failed;
		}
		
		if (!USAttributesComponent::IsActorAlive(TargetActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Failed checking TargetActor is alive"));
			return EBTNodeResult::Failed;
		}
		
		
		// Failed checks passed, we continue with the actual logic
		const FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator Rotation = Direction.Rotation();
		Rotation.Pitch += FMath::RandRange(-MaxRangedAttackSpread/2, MaxRangedAttackSpread);
		Rotation.Yaw   += FMath::RandRange(-MaxRangedAttackSpread  , MaxRangedAttackSpread);

		// SPAWN PROJECTILE
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;
		
		const AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, Rotation, Params);

		
		return (NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Fail"));
	return EBTNodeResult::Failed;
}