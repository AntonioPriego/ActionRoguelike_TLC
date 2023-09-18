// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"



// Sets default values
ASPlayerState::ASPlayerState()
{
	Credits = 1000;
}


// Simple methods
int32 ASPlayerState::GetCredits() const					{ return Credits;    }
void  ASPlayerState::SetCredits   (const int32 Amount)	{ Credits = Amount;  }
void  ASPlayerState::AddCredits   (const int32 Amount)	{ Credits += Amount; }
void  ASPlayerState::RemoveCredits(const int32 Amount)	{ Credits -= Amount; }
