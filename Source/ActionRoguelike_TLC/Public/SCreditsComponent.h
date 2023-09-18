// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPlayerState.h"
#include "Components/ActorComponent.h"
#include "SCreditsComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActor*, CreditsOwner, float, NewAmount, float, Delta);

/** WARNING! This ActorComponent only works with ASPlayerState
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_TLC_API USCreditsComponent : public UActorComponent
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
public:
	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

protected:
	ASPlayerState* PlayerState;

	
/*********************************** METHODS *********************************/
public:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;
	
	/** [SPlayerState] Get credits */
	UFUNCTION(BlueprintCallable, Category=Credits)
	int32 GetCredits() const;
	
	/** [SPlayerState] Set total credits */
	UFUNCTION(BlueprintCallable, Category=Credits)
	void SetCredits(const int32 Amount);
	
	/** [SPlayerState] Add credits */
	UFUNCTION(BlueprintCallable, Category=Credits)
	void AddCredits(const int32 Amount);
	
	/** [SPlayerState] Remove credits */
	UFUNCTION(BlueprintCallable, Category=Credits)
	bool RemoveCredits(const int32 Amount);

	/** [SPlayerState] Return if have the amount credits or more */
	UFUNCTION(BlueprintCallable, Category=Credits)
	bool HaveEnoughCredits(const int32 Amount) const;
};
