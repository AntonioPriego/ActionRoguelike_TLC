// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


// update next tick interval
void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI pawn and target actor
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (ensure(BlackboardComponent))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));

		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();

			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();

				if (ensure(AIPawn))
				{
					// Get if the player is on attack range for
					float DistanceTo  = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWithinRange = DistanceTo < 2000.0f;
					bool bInSightLine = false;
					
					if (bWithinRange)
					{
						bInSightLine = MyController->LineOfSightTo(TargetActor);
					}
					
					BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bInSightLine));
				}
			}
		}
	}
}
