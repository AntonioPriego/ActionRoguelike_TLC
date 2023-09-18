// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

/********************************* PROPERTIES ********************************/
protected:
	int32 Credits;
	
/*********************************** METHODS *********************************/
public:
	/** Sets default values */
	ASPlayerState();
	
	/** Get credits */
	int32 GetCredits() const;
	/** Set total credits */
	void SetCredits(const int32 Amount);
	/** Add credits */
	void AddCredits(const int32 Amount);
	/** Remove credits */
	void RemoveCredits(const int32 Amount);

};
