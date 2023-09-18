// Fill out your copyright notice in the Description page of Project Settings.

#include "SItemChest.h"


// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;
}


/* When interface function declared as BlueprintNativeEvent on UFUNCTION
   we have to add "_Implementation", bc we are using it on C++ but on BLUEPRINTS too */
// Definition of Interact function of SGameplayInterface on SItemChest
void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}
