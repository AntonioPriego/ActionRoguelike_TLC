// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoinPickUp.h"

#include "SCharacter.h"


// Sets default values
ASCoinPickUp::ASCoinPickUp()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set some default values
	CreditsEarned  = 1;
	CreditsCost    = 0;
	bIsReSpawnable = false;
	Adder          = 0;
}


void ASCoinPickUp::BeginPlay()
{
	Super::BeginPlay();
	TwistAnim();	// Timeline rotation anim on BP
}


// The actual behavior of the pick up item
bool ASCoinPickUp::OnPickUpBehavior(APawn* InstigatorPawn)
{
	const ASCharacter* CastedInstigator = Cast<ASCharacter>(InstigatorPawn);	

	if (CastedInstigator)
	{
		ASPlayerState* InstigatorPlayerState = CastedInstigator->GetSPlayerState();

		if (InstigatorPlayerState)
		{
			InstigatorPlayerState->AddCredits(CreditsEarned);
			return true;
		}
	}
	return false;
}