// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
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

void ASAICharacter::DebugDamage(const float DamageAmount)
{
	AttributesComponent->ApplyHealthChange(-DamageAmount);
}

float ASAICharacter::GetHealth()
{
	return AttributesComponent->GetHealth();
}
