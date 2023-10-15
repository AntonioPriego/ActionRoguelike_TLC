// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "SAttributesComponent.h"
#include "ActionRoguelike_TLC/ActionRoguelike_TLC.h"
#include "Net/UnrealNetwork.h"


USAction::USAction()
{
	// Set some values
	RageCost = 0.0f;
	bAutoStart = false;
	bIsRunning = false;
}


// Defines behavior at action start

void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Running: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* ActionComponent = GetOwningComponent();

	ActionComponent->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;
}


// Defines behavior at action end

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	USActionComponent* ActionComponent = GetOwningComponent();

	ActionComponent->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning = false;
}


// Simple GetWorld to bypass it to blueprint
UWorld* USAction::GetWorld() const
{
	const UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}


// Check if action is able to start
bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	const USActionComponent* ActionComponent = GetOwningComponent();
	USAttributesComponent* AttributesComponent = Instigator->FindComponentByClass<USAttributesComponent>();
	if (AttributesComponent && RageCost != 0.0f)
	{
		return (AttributesComponent->RageDecrease(RageCost));
	}
	return !ActionComponent->ActiveGameplayTags.HasAny(BlockedTags);
}


// bIsRunning getter
bool USAction::IsRunning_Implementation()
{
	return bIsRunning;
}

// Owning component getter
USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}


// Replication method for bIsRunning property
void USAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}


// ENGINE: Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties
void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, bIsRunning);
}
