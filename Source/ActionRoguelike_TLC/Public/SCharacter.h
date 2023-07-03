// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SAttributesComponent.h"
#include "SInteractionComponent.h"
#include "Components/InputComponent.h"

#include "SCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_TLC_API ASCharacter : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	ASCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called when inputs move is triggered
	void Move(const FInputActionValue& Value);
	
	// Called when inputs look (move camera) is triggered
	void Look(const FInputActionValue& Value);
	
	// Called when inputs primary attack is triggered
	void PrimaryAttack();
	
	// Called when inputs primary attack is triggered
	void SecondaryAttack();
	
	// Called when TimerHandle_*Attack time is elapsed 
	void Attack_TimeElapsed();
	
	// Called when inputs primary interact is triggered
	void PrimaryInteract();
	
	// Called when inputs primary attack is triggered
	void JumpTriggered();
	
	// Called when inputs primary attack is canceled
	void JumpCanceled();
	
	// Called when inputs primary attack is canceled
	void DashCast();


	
	/** Spring Arm Component to manage Camera collisions */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	/** Camera Component for player character */
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	/** The component for interaction management */
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	/** The component for attributes management */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributesComponent* AttributesComponent;


	
	/** Context input for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputMappingContext* PlayerContext;
	
	/** Move input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* MoveAction;
	
	/** Move camera input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* LookAction;
	
	/** Primary attack input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* PrimaryAttackAction;
	
	/** Secondary attack input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* SecondaryAttackAction;
	
	/** Primary interact input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* PrimaryInteractAction;
	
	/** Jump input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* JumpAction;
	
	/** Dash input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* DashAction;


	
	
	/** Save the current projectile casted */
	UPROPERTY(BlueprintReadOnly, Category=Attack)
	TSubclassOf<AActor> CurrentProjectile;
	
	/** To set primary project from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<AActor> PrimaryProjectileClass;
	
	/** To set secondary project from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<AActor> SecondProjectileClass;
	
	/** To set secondary project from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<AActor> DashProjectileClass;
	
	/** To set attack anim from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	UAnimMontage* AttackAnim;
	
	/** Manage the max distance to check hit actor on projectile trajectory */
	UPROPERTY(EditAnywhere, Category=Attack)
	float HitAttackRange;


	
	// Timer handles for attacks and abilities
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SecondaryAttack;
	FTimerHandle TimerHandle_Dash;


	
private:
	// Return true if any attack timer handle is pending
	bool IsAnyAttackTimerPending();
};
