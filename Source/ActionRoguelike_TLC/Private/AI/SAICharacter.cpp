// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
	// Default components
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributesComponent  = CreateDefaultSubobject<USAttributesComponent>("AttributesComponent");

	// Activate AI behavior on spawn and on placed actor (Default is only placed actor)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


// Internal function between Constructor and BeginPlay
void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributesComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}


// Called when pawn sees
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent();

		BBComponent->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}


// The broadcast function that notifies when Health changes on AttributesComponent
void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0  &&  Delta < 0)
	{
		// Stop BehaviorTree
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->GetBrainComponent()->StopLogic("Killed"); // BrainComponent is the base class of BTComponent
		}

		// Start Ragdoll
		GetMesh()->SetCollisionProfileName("Ragdoll");
		GetMesh()->SetAllBodiesSimulatePhysics(true);

		// Set lifespan
		SetLifeSpan(10.0f);
	}
}

/********************************** DEBUG **********************************/
void ASAICharacter::DebugDamage(const float DamageAmount)
{
	AttributesComponent->ApplyHealthChange(-DamageAmount);
}

float ASAICharacter::GetHealth()
{
	return AttributesComponent->GetHealth();
}
