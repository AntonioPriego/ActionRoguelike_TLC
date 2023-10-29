// Fill out your copyright notice in the Description page of Project Settings.

#include "SItemChest.h"
#include "Net/UnrealNetwork.h"


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

	// Server
	bReplicates = true;
}


// This method is auto called by unreal when our bLidOpened updates
void ASItemChest::OnRep_LidOpened()
{	
	//const float CurrentPitch = bLidOpened ? TargetPitch : 0.0f;	
	//LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
	OpenAnim(bLidOpened);
}


// USGameplayInterface: Definition of Interact function of SGameplayInterface on SItemChest
void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
	
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}


// Get the message is going to show on screen to interact
FText ASItemChest::GetInteractMessage_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}


// USGameplayInterface: Make necessary logic when Actor is loaded
void ASItemChest::OnActorLoaded_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{		
		OnRep_LidOpened();
	}
}


// ENGINE: Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties
// We do not have to declare in header file bc is auto-declared in SItemChest.generated.h
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened); // Whenever chest is opened, send it to all clients
}
