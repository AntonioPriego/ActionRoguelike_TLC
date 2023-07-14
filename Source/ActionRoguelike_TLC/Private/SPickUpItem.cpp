// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpItem.h"


// Sets default values
ASPickUpItem::ASPickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ASPickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ASPickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/* When interface function declared as BlueprintNativeEvent on UFUNCTION
we have to add "_Implementation", bc we are using it on C++ but on BLUEPRINTS too */
// Definition of Interact function of SGameplayInterface on PickUpItem
void ASPickUpItem::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);

	
}
