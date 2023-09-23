// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
	// Default components
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributesComponent  = CreateDefaultSubobject<USAttributesComponent>("AttributesComponent");
	ActionComponent      = CreateDefaultSubobject<USActionComponent>("ActionComponent");

	// Set FName material variables
	TimeToHitParamName = "TimeToHit";
	IsHealParamName    = "IsHeal";

	// Activate AI behavior on spawn and on placed actor (Default is only placed actor)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Collision setup for directional impulse on projectiles impact
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Credits default value
	CreditsValue = 20;
}


// Get credits amount that killer players receive when this actor is killed
int32 ASAICharacter::GetCreditsValue() const
{
	return CreditsValue;
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
	SetTargetActor(Pawn);	

	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}


// The broadcast function that notifies when Health changes on AttributesComponent
void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	
	// Damaged
	if (Delta <= 0)
	{
		OnCharacterDamaged(InstigatorActor, OwningComp, NewHealth, Delta);
	}
	
	// Healed
	if (Delta > 0)
	{
		OnCharacterHealed(InstigatorActor, OwningComp, NewHealth, Delta);
	}

	// TimeToHit act over the flash color when damaged or heal
	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

	// Create widget
	if (ActiveHealthBar == nullptr  &&  Delta != 0.0f)
	{
		ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this; // The way to explain the widget it serves this character
			ActiveHealthBar->AddToViewport();
		}
	}
}


// Logic when (this) character is damaged
void ASAICharacter::OnCharacterDamaged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	GetMesh()->SetScalarParameterValueOnMaterials(IsHealParamName, false);

		
	if (InstigatorActor != this)
	{
		// Change target to focus on the one is damaging this
		SetTargetActor(InstigatorActor);
	}
		
	if (NewHealth <= 0)
	{
		Dead();
	}
}


// Logic when (this) character is healed
void ASAICharacter::OnCharacterHealed(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	GetMesh()->SetScalarParameterValueOnMaterials(IsHealParamName, true);
}


// Set NewTarget as current TargetActor
void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent();
		BBComponent->SetValueAsObject("TargetActor", NewTarget);
	}
}


// Logic when (this) character dies
void ASAICharacter::Dead()
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

	// Deactivate collisions
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement(); // Without this line, the character just falls from anywhere bc collisions are disabled
}


/********************************** DEBUG **********************************/
void ASAICharacter::DebugDamage(const float DamageAmount)
{
	AttributesComponent->ApplyHealthChange(nullptr, -DamageAmount);
}

float ASAICharacter::GetHealth()
{
	return AttributesComponent->GetHealth();
}
