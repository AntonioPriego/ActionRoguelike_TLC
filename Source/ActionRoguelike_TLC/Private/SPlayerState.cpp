// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"



// Sets default values
ASPlayerState::ASPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
	Credits = 1000;
}

void ASPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Orange, FString::Printf(TEXT("[PlayerState] Owner: %s"), *GetOwner()->GetName()));
	if (!GetOwner()->GetName().Equals(FString("PlayerControllerBP_C_0")))
	{
		FString tmp = GetOwner()->GetName();
	}
}


// Remove credits
bool ASPlayerState::RemoveCredits(const int32 Amount)
{
	if (Credits >= Amount)
	{
		Credits -= Amount;
		
		OnCreditsChanged.Broadcast(GetOwner(), Credits, -Amount);
		return true;
	}
	return false;
}

// Add credits
void  ASPlayerState::AddCredits(const int32 Amount)
{
	if (Amount > 0)
	{
		Credits += Amount;
		OnCreditsChanged.Broadcast(GetOwner(), Credits, Amount);
	}
}


// Check if Amount is available
bool  ASPlayerState::HaveEnoughCredits(const int32 Amount) const
{
	return Credits >= Amount;
}


// Simple methods
int32 ASPlayerState::GetCredits() const	    				{ return Credits;    }
void  ASPlayerState::SetCredits       (const int32 Amount)	{ Credits = Amount;  }
