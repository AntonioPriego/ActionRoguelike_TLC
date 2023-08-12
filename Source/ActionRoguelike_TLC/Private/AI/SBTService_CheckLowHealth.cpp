// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values for this character's properties
USBTService_CheckLowHealth::USBTService_CheckLowHealth()
{
	// Set some values
	LowHealthPercent = 0.3f;
}


// Update next tick interval
void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

				
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();	// Get Blackboard
	const AAIController*  AIController        = OwnerComp.GetAIOwner();				// Get AIController to get Pawn (Querier)

	if (ensure(AIController && BlackboardComponent))
	{
		const APawn* AIPawn = AIController->GetPawn(); // Get Pawn (Querier)
		
		if (ensure(AIPawn))
		{
			UActorComponent* Component = AIPawn->GetComponentByClass(USAttributesComponent::StaticClass()); // Get Attribute Component as generic component
			const USAttributesComponent* AttributesComponent = Cast<USAttributesComponent>(Component);      // Cast component as AttributesComponent

			if (AttributesComponent)
			{
				// Finally! After thousands getters and casts, we can do the low health assignment
				BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName, IsLowHealth(AttributesComponent));
			}
		}
	}
}


// Get if AttributesComponent have low Health
bool USBTService_CheckLowHealth::IsLowHealth(const USAttributesComponent* AttributesComponent) const
{
	const float MaxHealth     = AttributesComponent->GetMaxHealth();
	const float CurrentHealth = AttributesComponent->GetHealth();
	const float CurrentHealthPercent = CurrentHealth/MaxHealth;
	

	return (CurrentHealthPercent < LowHealthPercent ? true : false);
}