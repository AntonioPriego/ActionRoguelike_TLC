// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributesComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributesComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_TLC_API USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	
protected:
	// EditAnywhere - edit in BP editor and per-instance in level.
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details'-panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// --
	// Category = "" - display only for detail panels and blueprint context menu.
	
	/** That's it, just the health as a float */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	float Health;

	/** The max Health value assignable */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	float MaxHealth;

	
/*********************************** METHODS *********************************/
public:	
	// Sets default values for this component's properties
	USAttributesComponent();

	/** Static function that returns AttributesComponent from an Actor */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	static USAttributesComponent* GetAttributes(const AActor* FromActor);

	/** Static function that returns if an Actor is currently Alive. If it fails, it will return also false */
	UFUNCTION(BlueprintCallable, Category=Attributes, meta=(DisplayName="IsAlive"))
	static bool IsActorAlive(const AActor* ActorToCheck);

	/** Apply the Delta increment/decrement to Health and return true if it was successful */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	/** Return if Health is greater than 0 */
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	/** Return if Health is equal to MaxHealth */
	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	/** Return Health */
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	/** Return MaxHealth */
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;


protected:
	/** Manages owner killed calling game-mode */
	void OnOwnerKilled(AActor* Killer) const;

	
/************************************ DEBUG **********************************/
public:
	/** DEBUG: To kill the actor with this AttributesComponent during testing */
	UFUNCTION(Exec)
	void Kill(AActor* InstigatorActor);
};
