// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAttributesComponent.h"
#include "SAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
	/** The component for attributes management */
	UPROPERTY(VisibleAnywhere, Category=Components)
	USAttributesComponent* AttributesComponent;
	
	/** Component to retrieve perception senses */
	UPROPERTY(VisibleAnywhere, Category=Components)
	UPawnSensingComponent* PawnSensingComponent;
	

/*********************************** METHODS *********************************/
public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	// Internal function between Constructor and BeginPlay
	void PostInitializeComponents() override;
	
	// Called when pawn sees
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	
	// The broadcast function that notifies when Health changes on AttributesComponent
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);
	

/*********************************** DEBUG ***********************************/
	UFUNCTION(BlueprintCallable)
	void DebugDamage(const float DamageAmount);
	UFUNCTION(BlueprintCallable)
	float GetHealth();
};
