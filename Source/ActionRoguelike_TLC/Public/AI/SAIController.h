// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API ASAIController : public AAIController
{
	GENERATED_BODY()

	
/********************************* PROPERTIES ********************************/
protected:
	UPROPERTY(EditDefaultsOnly, Category=AI)
	UBehaviorTree* BehaviorTree;

	
/*********************************** METHODS *********************************/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
