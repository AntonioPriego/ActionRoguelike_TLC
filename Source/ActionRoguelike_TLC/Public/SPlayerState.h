// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActor*, CreditsOwner, float, NewAmount, float, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

/********************************* PROPERTIES ********************************/
public:
	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;


protected:
	int32 Credits;

	
/*********************************** METHODS *********************************/
public:
	/** Sets default values */
	ASPlayerState();

	virtual void Tick(float DeltaSeconds) override;
	
	/** Get credits */
	UFUNCTION(BlueprintCallable)
	int32 GetCredits() const;
	
	/** Set total credits */
	UFUNCTION(BlueprintCallable)
	void SetCredits(const int32 Amount);
	
	/** Add credits */
	UFUNCTION(BlueprintCallable)
	void AddCredits(const int32 Amount);
	
	/** Remove credits */
	UFUNCTION(BlueprintCallable)
	bool RemoveCredits(const int32 Amount);
	
	/** Check if Amount is available */
	bool HaveEnoughCredits(const int32 Amount) const;

};
