// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGameplayInterface.h"
#include "SInteractionComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_TLC_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
// ...
/*********************************** METHODS *********************************/
public:
	/** Method to define logic on interaction */
	void PrimaryInteract();
};
