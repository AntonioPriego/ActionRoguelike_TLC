// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"

#include "SCharacter.h"
#include "SGameplayInterface.h"



//
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(0.12f); // Less frequency than default is enough and better for performance :)
	
	
	// Set some values
	TraceDistance    = 600.0f;
	TraceRadius      = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
}


// Method to define logic on interaction
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}


//
void USInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;	
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	
	TArray<FHitResult> Hits;
	float Radius = TraceRadius;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	
	// Clear reference before trying to fill
	FocusedActor = nullptr;

	// The trying to fill part itself
	for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		// First time an actor is focused by the component. Initialization
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		// Actual widget to viewport part
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
				Cast<ASCharacter>(GetOwner())->ChangeCrossHairToInteract();
				DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
			}
		}
	}
	else if (DefaultWidgetInstance)
	{
		DefaultWidgetInstance->RemoveFromParent();
		Cast<ASCharacter>(GetOwner())->ChangeCrossHairToNormal();
	}

}


// 
void USInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "[SInteractionComponent] No Focus Actor to Interact");
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}
