// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoinPickUp.h"



// Sets default values
ASCoinPickUp::ASCoinPickUp()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set some default values
	CreditsEarned = 1;
	CreditsCost   = 0;
	IsReSpawnable = false;
}


// The actual behavior of the pick up item
bool ASCoinPickUp::OnPickUpBehavior(APawn* InstigatorPawn)
{
	USCreditsComponent* CreditsComponent = InstigatorPawn->FindComponentByClass<USCreditsComponent>();	

	if (CreditsComponent)
	{
		CreditsComponent->AddCredits(CreditsEarned);
		
		return true;
	}

	return false;
}


// Called every frame
void ASCoinPickUp::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Magic numbers, temporal and arbitrary values
	float Yaw  = 180*DeltaSeconds;
	float Down = sin(Adder*5)*100;
	Adder += DeltaSeconds;
	
	if (GetActorRotation().Yaw > 0)
	{
		Yaw  *= 2.5;
	}
	AddActorLocalRotation(FRotator(0, Yaw,0));
	AddActorLocalOffset  (FVector(0,0,Down*DeltaSeconds));
}