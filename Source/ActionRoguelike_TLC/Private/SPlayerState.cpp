// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"



// Sets default values
ASPlayerState::ASPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
	Credits = 1000;
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
