// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpItem.h"


// Sets default values
ASPickUpItem::ASPickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Set up MeshComponent default, as a root and collision profile
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName("OverlapOnlyPawn");

	// Set up some values
	RespawnSeconds    = 10.0f;
	IsReSpawnable     = true;
}


// Called when the game starts or when spawned
void ASPickUpItem::BeginPlay()
{
	Super::BeginPlay();
}


// Definition of Interact function of SGameplayInterface on PickUpItem
void ASPickUpItem::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);


	if (IsReSpawnable) {
		// Disabled and reEnabled in RespawnSeconds seconds
		Disable();		
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPickUpItem::Enable, RespawnSeconds);
	}
	else {
		// Clear timer due to possible inconsistencies and destroy bc is not respawnable
		GetWorldTimerManager().ClearTimer(TimerHandle_Respawn);		
		Destroy();
	}
}


// Set Actor enabled: visibility and collisions
void ASPickUpItem::Enable()
{
	// Clear timer due to possible inconsistencies
	GetWorldTimerManager().ClearTimer(TimerHandle_Respawn);
	
	// Visibility
	MeshComponent->SetVisibility(true);

	// Collisions
	SetActorEnableCollision(true);
}


// Set Actor disabled: visibility and collisions
void ASPickUpItem::Disable()
{
	// Clear timer due to possible inconsistencies
	GetWorldTimerManager().ClearTimer(TimerHandle_Respawn);
	
	// Visibility
	MeshComponent->SetVisibility(false);

	// Collisions
	SetActorEnableCollision(false);
}
