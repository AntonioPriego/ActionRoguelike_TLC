// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "ActionRoguelike_TLC/ActionRoguelike_TLC.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	PrimaryComponentTick.bCanEverTick = true;

	// Server replication
	SetIsReplicatedByDefault(true);
}


// Where DefaultActions array meets Actions array
void USActionComponent::InitializeDefaultActions()
{
	for (const TSubclassOf<USAction> Action : DefaultActions)
	{
		AddAction(GetOwner(), Action);
	}
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server only
	if (GetOwner()->HasAuthority())
	{
		InitializeDefaultActions();
	}
}


// Logic needed to stay sync with server on start action 
void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}


// Logic needed to stay sync with server on stop action 
void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// const FString DebugMessage = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Emerald, DebugMessage);

	// Draw all actions
	for (USAction* Action : Actions)
	{
		const FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		const FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*GetNameSafe(Action),
			Action->IsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(GetOuter())
		);

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}


// Component "learn" a new action
void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}


// Component "forget" an action from Actions
void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(ActionToRemove);
}


// Call to start the action by its name
bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMessage = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FailedMessage);
				continue;
			}
			
			// Is client? | HasAuthority() returns if this is running on Server
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			// Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));
			{
				// Scoped within the curly braces. the _FSTRING variant adds additional tracing overhead due to grabbing the class name every time
				SCOPED_NAMED_EVENT_FSTRING(Action->GetClass()->GetName(), FColor::White);

				Action->StartAction(Instigator);
			}
			
			return true;
		}
	}

	return false;
}


// Call to stop the action by its name
bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				// Is client? | HasAuthority() returns if this is running on Server
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}


// ENGINE: Allows a component to replicate other subobject on the actor
bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);	// We write into the Boolean only if the ReplicateSubobject returns TRUE.
																							// That way we can track if any of the function calls returned True and there we "WroteSomething".
		}
	}

	return WroteSomething;
}


// ENGINE: Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties
void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}