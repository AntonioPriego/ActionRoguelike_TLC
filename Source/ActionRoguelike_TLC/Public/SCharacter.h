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
	
	
	/** To set primary project from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<AActor> PrimaryProjectileClass;
	
	/** To set secondary project from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<AActor> SecondaryProjectileClass;
	
	/** To set secondary project from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<AActor> DashProjectileClass;
	
	/** To set attack anim from editor */
	UPROPERTY(EditAnywhere, Category=Attack)
	UAnimMontage* AttackAnim;
	
	/** Manage the max distance to check hit actor on projectile trajectory */
	UPROPERTY(EditAnywhere, Category=Attack)
	float HitAttackRange;

	/** Manage the max distance to check hit actor on projectile trajectory */
	UPROPERTY(VisibleAnywhere, Category=Attack)
	FName HandSocketName;
	
	/** FName for material variable that controls hit cue */
	UPROPERTY(VisibleAnywhere, Category=Effects)
	FName TimeToHitParamName;
	
	/** FName for material variable that controls hit marked as Heal or as Damage */
	UPROPERTY(VisibleAnywhere, Category=Effects)
	FName IsHealParamName;


	
	// Timer handles for attacks and abilities
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SecondaryAttack;
	FTimerHandle TimerHandle_Dash;

	
/*********************************** METHODS *********************************/
public:
	// Sets default values for this character's properties
	ASCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Getter for Mesh
	USkeletalMeshComponent* GetSKMesh() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Internal function between Constructor and BeginPlay
	virtual void PostInitializeComponents() override;
	
	// Called when inputs move is triggered
	void Move(const FInputActionValue& Value);
	
	// Called when inputs look (move camera) is triggered
	void Look(const FInputActionValue& Value);
	
	// Primary attack method
	void PrimaryAttack();
	
	// Secondary attack method
	void SecondaryAttack();
	
	// Dash ability method
	void DashCast();
	
	// Called when TimerHandle_PrimaryAttack time is elapsed 
	void PrimaryAttack_TimeElapsed();

	// Called when TimerHandle_SecondaryAttack time is elapsed 
	void SecondaryAttack_TimeElapsed();

	// Called when TimerHandle_Dash time is elapsed 
	void DashAbility_TimeElapsed();

	// Called to spawn projectiles from player
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);	
	
	// Called when inputs primary interact is triggered
	void PrimaryInteract();
	
	// Called when inputs jump is triggered
	void JumpTriggered();
	
	// Called when inputs jump is canceled
	void JumpCanceled();


	// The broadcast function that notifies when Health changes on AttributesComponent
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	// Method called when Mesh begin overlap with another collision actor
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult &HitResult);

private:
	// Return true if any attack timer handle is pending
	bool IsAnyAttackTimerPending();

	
/************************************ DEBUG **********************************/
public:
	/** DEBUG: To quick heal on testing */
	UFUNCTION(Exec)
	void HealSelf(const float Amount=1000);
	
};
