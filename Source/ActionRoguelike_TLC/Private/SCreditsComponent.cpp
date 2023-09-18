// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditsComponent.h"
#include "GameFramework/Character.h"



// Called when the game starts or when spawned
void USCreditsComponent::BeginPlay()
{
	Super::BeginPlay();

	// Check player have ASPlayerState as PlayerState
	PlayerState = nullptr;
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (IsValid(Owner))
	{
		ASPlayerState* CompatiblePlayerState = Cast<ASPlayerState>(Owner->GetPlayerState());
		if (CompatiblePlayerState)
		{
			PlayerState = CompatiblePlayerState;
		}
		else
		{			
			UE_LOG(LogTemp, Warning, TEXT("Could not initialize CreditsComponent from %s: PlayerState not compatible (must be ASPlayerState)"),
				   *Owner->GetName());
		}
	}
	else
	{		
		UE_LOG(LogTemp, Warning, TEXT("Could not initialize CreditsComponent: Not valid ACharacter owner"));
	}
}


// Get credits
int32 USCreditsComponent::GetCredits() const
{
	return (PlayerState ? PlayerState->GetCredits() : 0);
}


// Set total credits
void USCreditsComponent::SetCredits(const int32 Amount)
{
	if (PlayerState)
	{
		PlayerState->SetCredits(Amount);
		OnCreditsChanged.Broadcast(PlayerState->GetOwner(), GetCredits(), Amount);
	}
}


// Add credits
void USCreditsComponent::AddCredits(const int32 Amount)
{
	if (PlayerState)
	{
		PlayerState->AddCredits(Amount);
		OnCreditsChanged.Broadcast(PlayerState->GetOwner(), GetCredits(), Amount);
	}
}


// Remove credits
bool USCreditsComponent::RemoveCredits(const int32 Amount)
{
	UE_LOG(LogTemp, Log, TEXT("Entra a RemoveCredits (Cantidad:%d)"), Amount);
	if (PlayerState  &&  HaveEnoughCredits(Amount))
	{
		PlayerState->RemoveCredits(Amount);
		OnCreditsChanged.Broadcast(PlayerState->GetOwner(), GetCredits(), Amount);

		return true;
	}

	return false;
}


// Return if have the amount credits or more
bool USCreditsComponent::HaveEnoughCredits(const int32 Amount) const
{
	return GetCredits() >= Amount;
}
