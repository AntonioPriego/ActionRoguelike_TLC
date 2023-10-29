// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(SPlayerController)


// Add or remove from viewport the pause menu widget
void ASPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance->GetVisibility() == ESlateVisibility::Visible)
	{
		UnsetPause();
	}
	else
	{
		SetPause();
	}
}

void ASPlayerController::SetPause()
{
	PauseMenuInstance->SetVisibility(ESlateVisibility::Visible);

	bShowMouseCursor = true;
	
	FInputModeGameAndUI InputModeGameAndUI = FInputModeGameAndUI();
	InputModeGameAndUI.SetHideCursorDuringCapture(false);
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeGameAndUI);

	// Single-player only
	if (GetWorld()->IsNetMode(NM_Standalone))
	{
		UGameplayStatics::SetGamePaused(this, true);
	}

	// Disable input
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 100);
	if (PlayerCharacter)
	{
		PlayerCharacter->DisableInput(this);
	}
}

void ASPlayerController::UnsetPause()
{
	PauseMenuInstance->SetVisibility(ESlateVisibility::Collapsed);

	bShowMouseCursor = false;
	
	SetInputMode(FInputModeGameOnly().SetConsumeCaptureMouseDown(false));

	// Single-player only
	if (GetWorld()->IsNetMode(NM_Standalone))
	{
		UGameplayStatics::SetGamePaused(this, false);
	}

	// Enable input
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		PlayerCharacter->EnableInput(this);
	}
}


// Set up input enhanced system to access player input
void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(TogglePauseAction, ETriggerEvent::Started, this, &ASPlayerController::TogglePauseMenu);
	}
}


// Set attached pawn for this PlayerController
void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (PlayerState)
	{
		BeginPlayingState();
	}

	OnPawnChanged.Broadcast(InPawn);
}


// Called when player controller is ready to begin playing
void ASPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}


// Replication for PlayerState
void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}