// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SActionComponent.h"
#include "SAttributesComponent.h"
#include "SInteractionComponent.h"
#include "SPlayerState.h"
#include "Components/InputComponent.h"
#include "SCharacter.generated.h"


UCLASS()
class ACTIONROGUELIKE_TLC_API ASCharacter : public ACharacter
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
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

	/** The component for credits management */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USActionComponent* ActionComponent;

	
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
	
	/** Sprint input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* SprintAction;
	
	/** Dash input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* DashAction;

	/** Parry input action for player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* ParryAction;
	
	/** FName for material variable that controls hit cue */
	UPROPERTY(VisibleAnywhere, Category=Effects)
	FName TimeToHitParamName;
	
	/** FName for material variable that controls hit marked as Heal or as Damage */
	UPROPERTY(VisibleAnywhere, Category=Effects)
	FName IsHealParamName;

	
/*********************************** METHODS *********************************/
public:
	/** Sets default values for this character's properties */
	ASCharacter();
	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Getter for Mesh */
	USkeletalMeshComponent* GetSKMesh() const;

	/** Get our ASPlayerState */
	UFUNCTION(BlueprintCallable, Category=PlayerState)
	ASPlayerState* GetSPlayerState() const;
	
	/** Override for Pawn method to set the PawnView we want */
	virtual FVector GetPawnViewLocation() const override;
	
	/** Return Camera forward vector */
	FVector GetPawnViewForwardVector() const;


protected:	
	/** Internal function between Constructor and BeginPlay */
	virtual void PostInitializeComponents() override;
	
	/** Called when inputs move is triggered */
	void Move(const FInputActionValue& Value);
	
	/** Called when inputs look (move camera) is triggered */
	void Look(const FInputActionValue& Value);
	
	/** Primary attack method */
	void PrimaryAttack();
	
	/** Secondary attack method */
	void SecondaryAttack();
	
	/** Dash ability method */
	void DashCast();
	
	/** Called when inputs primary interact is triggered */
	void PrimaryInteract();
	
	/** Called when inputs jump is triggered */
	void JumpTriggered();
	
	/** Called when inputs jump is canceled */
	void JumpCanceled();

	/** Character will move faster */
	void SprintStart();
	
	/** Character return to normal moving velocity */
	void SprintStop();


	/** The broadcast function that notifies when Health changes on AttributesComponent */
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	/** Method called when Mesh begin overlap with another collision actor */
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult &HitResult);

	/** Logic when (this) character dies */
	void Dead();

	
/************************************ DEBUG **********************************/
public:
	/** DEBUG: To quick heal on testing */
	UFUNCTION(Exec)
	void HealSelf(const float Amount=1000);
	
};