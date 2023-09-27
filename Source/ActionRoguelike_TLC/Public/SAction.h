// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;


/**
 * 
 */
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

	bool bIsRunning;

	
/*********************************** METHODS *********************************/
public:
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


protected:
	/** Owning component getter */
	UFUNCTION(BlueprintCallable, Category=Action)
	USActionComponent* GetOwningComponent() const;
	
};
