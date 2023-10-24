// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/** Interface to manage Interaction on gameplay */
class ACTIONROGUELIKE_TLC_API ISGameplayInterface
{
	GENERATED_BODY()
	
/********************************* PROPERTIES ********************************/
// ...
/*********************************** METHODS *********************************/
public:
	/** To establish the communication between the Instigator and the Actor instigated (that implements this interface) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
	
	/** Get the message is going to show on screen to interact */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractMessage(APawn* InstigatorPawn);

	/** Make necessary logic when Actor is loaded */
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();
};
