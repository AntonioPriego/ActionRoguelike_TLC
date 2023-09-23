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
protected:
	/**  */
	UPROPERTY()
	TArray<USAction*> Actions;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tags)
	FGameplayTagContainer ActiveGameplayTags;;

	/** Granted abilities at game start */
	UPROPERTY(EditAnywhere, Category=Actions)
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	
/*********************************** METHODS *********************************/
public:
	/** Sets default values for this component's properties */
	USActionComponent();

	/** Where DefaultActions array meets Actions array */
	void InitializeDefaultActions();

	/** */
	UFUNCTION(BlueprintCallable, Category=Actions)
	void AddAction(TSubclassOf<USAction> ActionClass);
	
	/** */
	UFUNCTION(BlueprintCallable, Category=Actions)
	bool StartActionByName (AActor* Instigator, FName ActionName);

	/** */
	UFUNCTION(BlueprintCallable, Category=Actions)
	bool StopActionByName(AActor* Instigator, FName ActionName);

	
protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

public:	
	/** Called every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
