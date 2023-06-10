// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"

#include "SCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when inputs move is triggered
	void Move(const FInputActionValue& Value);
	// Called when inputs move is triggered
	void Look(const FInputActionValue& Value);

	/** Spring Arm Component to manage Camera collisions */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	/** Camera Component for player character */
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	// /** Context input for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputMappingContext* PlayerContext;
	
	// /** Move input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* MoveAction;
	// /** Move camera input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* LookAction;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
