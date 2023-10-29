// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASPlayerState::ASPlayerState()
{
	// Set some values
	PrimaryActorTick.bCanEverTick = true;
	Credits           = 1000;
	RoundPersonalBest = 0;
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


// Get RoundPersonalBest
int32 ASPlayerState::GetRoundPersonalBest() const
{
	return RoundPersonalBest;
}


// Set RoundPersonalBest
void ASPlayerState::SetRoundPersonalBest(const int32 newPB)
{
	RoundPersonalBest = newPB;
}

void ASPlayerState::OnRep_Credits(const int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}


// 
void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits           = Credits;
		SaveObject->RoundPersonalBest = RoundPersonalBest;
	}
}


// 
void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		AddCredits(SaveObject->Credits);
		RoundPersonalBest = SaveObject->RoundPersonalBest;
	}
}


//
void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPlayerState, Credits);
}


// Simple methods
int32 ASPlayerState::GetCredits() const	    		 { return Credits;    }
void  ASPlayerState::SetCredits (const int32 Amount) { Credits = Amount;  }
