// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_TLC_API USAction : public UObject
{
	GENERATED_BODY()



/********************************* PROPERTIES ********************************/
public:
	/** */
	UPROPERTY(EditDefaultsOnly, Category=Action)
	FName ActionName;

	
/*********************************** METHODS *********************************/
public:
	/** Defines behavior at action start */
	UFUNCTION(BlueprintNativeEvent, Category=Action)
	void StartAction(AActor* Instigator);
	
	/** Defines behavior at action end */
	UFUNCTION(BlueprintNativeEvent, Category=Action)
	void StopAction(AActor* Instigator);

	/** Simple GetWorld to bypass it to blueprint */
	UFUNCTION(BlueprintCallable)
	virtual UWorld* GetWorld() const override;
};
