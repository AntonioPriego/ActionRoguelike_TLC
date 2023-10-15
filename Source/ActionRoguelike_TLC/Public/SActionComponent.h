// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_TLC_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()


	
/********************************* PROPERTIES ********************************/
public:
	/** Contain current active tags */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tags)
	FGameplayTagContainer ActiveGameplayTags;

	
protected:
	/** All the actions this component is able to do */
	UPROPERTY(Replicated)
	TArray<USAction*> Actions;

	/** Granted abilities at game start */
	UPROPERTY(EditAnywhere, Category=Actions)
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	
/*********************************** METHODS *********************************/
public:	
	/** Sets default values for this component properties */
	USActionComponent();

	/** Where DefaultActions array meets Actions array */
	void InitializeDefaultActions();

	/** Component "learn" a new action */
	UFUNCTION(BlueprintCallable, Category=Actions)
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);
	
	/** Component "forget" an action from Actions */
	UFUNCTION(BlueprintCallable, Category=Actions)
	void RemoveAction(USAction* ActionToRemove);
	
	/** Call to start the action by its name */
	UFUNCTION(BlueprintCallable, Category=Actions)
	bool StartActionByName (AActor* Instigator, FName ActionName);

	/** Call to stop the action by its name */
	UFUNCTION(BlueprintCallable, Category=Actions)
	bool StopActionByName(AActor* Instigator, FName ActionName);

	/** ENGINE:
	* Allows a component to replicate other subobject on the actor.
	* Must return true if any data gets serialized into the bunch.
	* This method is used only when bReplicateUsingRegisteredSubObjectList is false.
	* Otherwise this function is not called and only the ReplicatedSubObjects list is used.
	*/
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	
protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Logic needed to stay sync with server */
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

public:	
	/** Called every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
