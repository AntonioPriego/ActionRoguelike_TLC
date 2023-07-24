// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	/** Component to retrieve perception senses */
	UPROPERTY(VisibleAnywhere, Category=Components)
	UPawnSensingComponent* PawnSensingComponent;

	// Internal function between Constructor and BeginPlay
	void PostInitializeComponents() override;
	
	// Called when pawn sees
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
