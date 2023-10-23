// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;


/** To ensure we are passing replicated both elements instead of passing it separately */
USTRUCT()
struct FActionRepData
{
	GENERATED_BODY();

public:
	UPROPERTY()
	bool bIsRunning;
	
	UPROPERTY()
	AActor* Instigator;
};


UCLASS(Blueprintable)
class ACTIONROGUELIKE_TLC_API USAction : public UObject
{
	GENERATED_BODY()



/********************************* PROPERTIES ********************************/
public:
	/** Name for Action (Remain it is frequently used, so give it a nice name) */
	UPROPERTY(EditDefaultsOnly, Category=Action)
	FName ActionName;

	/** Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tags)
	FGameplayTagContainer GrantTags;

	/** Action can only start if owningActor has none of these tags applied */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tags)
	FGameplayTagContainer BlockedTags;

	/** Start immediately when added to an action component */
	UPROPERTY(EditDefaultsOnly, Category=Action)
	bool bAutoStart;

	/** The amount of range the action costs */
	UPROPERTY(EditDefaultsOnly, Category=Action)
	float RageCost;


protected:
	/** The flag which signal the running status for the SAction */
	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	/** Icon showed on widgets for player clue */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=UI)
	UTexture2D* Icon;

	/** Color for icon showed on widgets for player clue */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=UI)
	FColor IconColor;

	/** Saves the TimeSeconds on Action started */
	UPROPERTY(Replicated)
	float TimeStarted;

		
/*********************************** METHODS *********************************/
public:
	USAction();
	
	/** Defines behavior at action start */
	UFUNCTION(BlueprintNativeEvent, Category=Action)
	void StartAction(AActor* Instigator);
	
	/** Defines behavior at action end */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Action)
	void StopAction(AActor* Instigator);

	/** Simple GetWorld to bypass it to blueprint */
	UFUNCTION(BlueprintCallable)
	virtual UWorld* GetWorld() const override;

	/** Check if action is able to start */
	UFUNCTION(BlueprintNativeEvent, Category=Action)
	bool CanStart(AActor* Instigator);
	
	/** bIsRunning getter */
	UFUNCTION(BlueprintNativeEvent, Category=Action)
	bool IsRunning();

	/** ENGINE: By default this return false */
	virtual bool IsSupportedForNetworking() const override {return true;}


protected:
	/** Owning component getter */
	UFUNCTION(BlueprintCallable, Category=Action)
	USActionComponent* GetOwningComponent() const;

	/** Replication method for bIsRunning property */
	UFUNCTION()
	void OnRep_RepData();
	
};
