// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAttributesComponent.h"
#include "STargetDummy.generated.h"


UCLASS()
class ACTIONROGUELIKE_TLC_API ASTargetDummy : public AActor
{
	GENERATED_BODY()

	
/********************************* PROPERTIES ********************************/
protected:
	/** The component for attributes management */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USAttributesComponent> AttributeComp;

	/** Class visible mesh */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	
/*********************************** METHODS *********************************/
public:
	// Sets default values for this actor's properties
	ASTargetDummy();

	// Internal function between Constructor and BeginPlay
	virtual void PostInitializeComponents() override;

	
protected:	
	/** OnHealthChanged method for AttributesComponent */
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

};