// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributesComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributesComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRageChanged, USAttributesComponent*, OwningComp, float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_TLC_API USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;

	
protected:
	/** That's it, just the health as a float */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	float Health;

	/** The max Health value assignable */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	float MaxHealth;

	/** Rage is an attribute used for certain abilities */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	float Rage;

	/** The max Rage value assignable */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	float MaxRage;

	/** The max Rage value assignable */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	float RatioDamageRage;

	
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
	UFUNCTION(BlueprintCallable, Category=Attributes)
	bool IsAlive() const;

	/** Return if Health is equal to MaxHealth */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	bool IsFullHealth() const;

	/** Return Health */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	float GetHealth() const;

	/** Return MaxHealth */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	float GetMaxHealth() const;

	/** Return Rage */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	float GetRage() const;

	/** Return MaxRage */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	float GetMaxRage() const;

	/** Set RatioDamageRage */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	void SetRatioDamageRage(const float NewRatio);

	/** Return RatioDamageRage */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	float GetRatioDamageRage() const;
	
	/** Add Rage amount depending on damage received */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	void RageIncrease(float DamageReceived);
	
	/** Add Rage amount depending on damage received */
	UFUNCTION(BlueprintCallable, Category=Attributes)
	bool RageDecrease(const float DecreaseAmount);


protected:
	/** Manages owner killed calling game-mode */
	void OnOwnerKilled(AActor* Killer) const;

	
/************************************ DEBUG **********************************/
public:
	/** DEBUG: To kill the actor with this AttributesComponent during testing */
	UFUNCTION(Exec)
	void Kill(AActor* InstigatorActor);
};
