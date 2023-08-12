// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"
#include "AIController.h"
#include "SAttributesComponent.h"


// Starts this task, should return Succeeded, Failed or InProgress
EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool TaskPassedSuccessfully = false;	
	const AAIController*  AIController = OwnerComp.GetAIOwner(); // Get AIController to get Pawn (Querier)

	if (AIController)
	{
		const APawn* AIPawn = AIController->GetPawn(); // Get Pawn (Querier)
	
		if (AIPawn)
		{
			UActorComponent* PawnComponent = AIPawn->GetComponentByClass(USAttributesComponent::StaticClass());	// Get Attribute Component as generic component
			USAttributesComponent* AttributesComponent = Cast<USAttributesComponent>(PawnComponent);			// Cast component as AttributesComponent

			// The task itself is only this, apply the heal
			if (AttributesComponent)
			{
				AttributesComponent->ApplyHealthChange(HealAmount);
				TaskPassedSuccessfully = true;
			}
		}
	}
	
	// If TaskPassedSuccessfully still false, the task failed
	if (!TaskPassedSuccessfully)
	{
		return EBTNodeResult::Failed;
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
