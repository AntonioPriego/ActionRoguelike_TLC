// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SWorldUserWidget.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_TLC_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
	/** This actor pointer, target the thing with the component is going to potentially interact */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> FocusedActor;

	/** The distance from the player is going to check the component */
	UPROPERTY(EditDefaultsOnly, Category=Trace)
	float TraceDistance;
	
	/** The radius of the trace that is going to travel from the player to the TraceDistance position */
	UPROPERTY(EditDefaultsOnly, Category=Trace)
	float TraceRadius;

	/** The radius of the trace that is going to travel from the player to the TraceDistance position */
	UPROPERTY(EditDefaultsOnly, Category=Trace)
	TEnumAsByte<ECollisionChannel> CollisionChannel; // TEnumAsByte is needed bc we have to deal with it as a raw enum

	/** */
	UPROPERTY(EditDefaultsOnly, Category=UI)
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	/** */
	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;
	
	
/*********************************** METHODS *********************************/
public:
	USInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Method to define logic on interaction */
	void PrimaryInteract();

	
protected:
	/** Interaction but sync with server */
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
	/** Search for something to interact */
	void FindBestInteractable();
};
