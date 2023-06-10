// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"



// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up CameraComponent (So, SpringArmComponent too)
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent"); // SpringArm
	CameraComponent    = CreateDefaultSubobject<UCameraComponent>("CameraComponent");		// Camera
	SpringArmComponent->SetupAttachment(RootComponent);									// Attachment SpringArm->RootComponent
	CameraComponent->SetupAttachment(SpringArmComponent);									// Attachment Camera->SpringArm
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set up Input
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(PlayerContext,0);
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		
	}
}

// Called when input
void ASCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D CurrentValue = Value.Get<FVector2D>();

	if (CurrentValue.X)
		AddMovementInput(GetActorForwardVector(), CurrentValue.X);
	if (CurrentValue.Y)
		AddMovementInput(GetActorForwardVector().RotateAngleAxis(90,FVector(0,0,1)), CurrentValue.Y);

	GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, FString::Printf(TEXT("%f | %f"), CurrentValue.X, CurrentValue.Y));
}

void ASCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D CurrentValue = Value.Get<FVector2D>();

	if (CurrentValue.X)
		AddControllerYawInput(CurrentValue.X);
	if (CurrentValue.Y)
		AddControllerPitchInput(CurrentValue.Y);

	GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, FString::Printf(TEXT("%f | %f"), CurrentValue.X, CurrentValue.Y));
}
