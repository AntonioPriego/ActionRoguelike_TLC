// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"


// Defines behavior at action start
void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	USActionComponent* ActionComponent = GetOwningComponent();
	
	ActionComponent->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;
}


// Defines behavior at action end
void USAction::StopAction_Implementation(AActor* Instigator)
{	
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensure(bIsRunning);

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
