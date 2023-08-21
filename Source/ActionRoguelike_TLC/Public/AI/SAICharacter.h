// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAttributesComponent.h"
#include "SWorldUserWidget.h"
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

	/** Widget to show as health bar */
	UPROPERTY(EditDefaultsOnly, Category=UI)
	TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;

	/** FName for material variable that controls hit cue */
	UPROPERTY(VisibleAnywhere, Category=Effects)
	FName TimeToHitParamName;
	
	/** FName for material variable that controls hit marked as Heal or as Damage */
	UPROPERTY(VisibleAnywhere, Category=Effects)
	FName IsHealParamName;

	/** A reference of the created widget for health bar */
	USWorldUserWidget* ActiveHealthBar;
	

/*********************************** METHODS *********************************/
public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	/**  Internal function between Constructor and BeginPlay */
	void PostInitializeComponents() override;
	
	/**  Called when pawn sees */
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	
	/** The broadcast function that notifies when Health changes on AttributesComponent */
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	/** Logic when (this) character is damaged */
	UFUNCTION()
	void OnCharacterDamaged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	/** Logic when (this) character is healed */
	UFUNCTION()
	void OnCharacterHealed(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	/** Set NewTarget as current TargetActor */
	void SetTargetActor(AActor* NewTarget);

	/** Logic when (this) character dies */
	void Dead();
	

/*********************************** DEBUG ***********************************/
	UFUNCTION(BlueprintCallable)
	void DebugDamage(const float DamageAmount);
	UFUNCTION(BlueprintCallable)
	float GetHealth();
};
