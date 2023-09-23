// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "SPlayerState.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up Actor Components
	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");
	AttributesComponent  = CreateDefaultSubobject<USAttributesComponent> ("AttributesComponent" );
	CreditsComponent     = CreateDefaultSubobject<USCreditsComponent>    ("CreditsComponent"    );
	ActionComponent      = CreateDefaultSubobject<USActionComponent>     ("ActionComponent"     );

	// Set up CameraComponent (So, SpringArmComponent too)
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent"); // SpringArm
	CameraComponent    = CreateDefaultSubobject<UCameraComponent>("CameraComponent");		// Camera
	SpringArmComponent->SetupAttachment(RootComponent);									// Attachment SpringArm->RootComponent
	CameraComponent->SetupAttachment(SpringArmComponent);									// Attachment Camera->SpringArm

	// Some constructor values
	SpringArmComponent->bUsePawnControlRotation = true;			///  
	bUseControllerRotationYaw = false;							// https://drive.google.com/file/d/1QDxeYIUHOry3bJtOwmN2_SAaEdPWIXTy/view?usp=sharing
	GetCharacterMovement()->bOrientRotationToMovement = true;	///

	// Set FName material variables
	TimeToHitParamName = "TimeToHit";
	IsHealParamName    = "IsHeal";

    // Activate OverlapEvents on player mesh
    GetMesh()->SetGenerateOverlapEvents(true);
}


// Internal function between Constructor and BeginPlay
void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributesComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ASCharacter::OnActorBeginOverlap);
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	constexpr float DrawScale = 100.0f;
	constexpr float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	const FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow,
		                      false, 0.0f, 0, Thickness);

	const FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green,
	                          false, 0.0f, 0, Thickness);
}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up Input
	const APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController)
	{
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (Subsystem)
		{
			Subsystem->AddMappingContext(PlayerContext,0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,            ETriggerEvent::Triggered, this, &ASCharacter::Move           );
		EnhancedInputComponent->BindAction(LookAction,            ETriggerEvent::Triggered, this, &ASCharacter::Look           );
		EnhancedInputComponent->BindAction(PrimaryAttackAction,   ETriggerEvent::Started,   this, &ASCharacter::PrimaryAttack  );
		EnhancedInputComponent->BindAction(SecondaryAttackAction, ETriggerEvent::Started,   this, &ASCharacter::SecondaryAttack);
		EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started,   this, &ASCharacter::PrimaryInteract);
		EnhancedInputComponent->BindAction(JumpAction,            ETriggerEvent::Started,   this, &ASCharacter::JumpTriggered  );		
		EnhancedInputComponent->BindAction(JumpAction,            ETriggerEvent::Canceled,  this, &ASCharacter::JumpCanceled   );	
		EnhancedInputComponent->BindAction(SprintAction,          ETriggerEvent::Started,   this, &ASCharacter::SprintStart    );		
		EnhancedInputComponent->BindAction(SprintAction,          ETriggerEvent::Completed, this, &ASCharacter::SprintStop     );		
		EnhancedInputComponent->BindAction(DashAction,            ETriggerEvent::Started,   this, &ASCharacter::DashCast       );	
		EnhancedInputComponent->BindAction(ParryAction,          ETriggerEvent::Completed, this, &ASCharacter::SprintStop      );
	}
}


// Getter for Mesh
USkeletalMeshComponent* ASCharacter::GetSKMesh() const
{
	return GetMesh();
}


// Get our ASPlayerState
ASPlayerState* ASCharacter::GetSPlayerState() const
{
	return Cast<ASPlayerState>(GetPlayerState());
}


// Called when inputs move is triggered
void ASCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D CurrentValue = Value.Get<FVector2D>();
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll  = 0.0f;	

	if (CurrentValue.X)
	{
		AddMovementInput(ControlRotation.Vector(), CurrentValue.X);
	}
	if (CurrentValue.Y)
	{
		AddMovementInput(ControlRotation.Vector().RotateAngleAxis(90,FVector(0,0,1)), CurrentValue.Y);
	}

	GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, FString::Printf(TEXT("%f | %f"), CurrentValue.X, CurrentValue.Y));
}


// Called when inputs look (move camera) is triggered
void ASCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D CurrentValue = Value.Get<FVector2D>();

	if (CurrentValue.X)
	{
		AddControllerYawInput(CurrentValue.X);
	}
	if (CurrentValue.Y)
	{
		AddControllerPitchInput(-CurrentValue.Y);
	}
}


// Primary attack method
void ASCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, FName("PrimaryAttack"));
}


// Secondary attack method
void ASCharacter::SecondaryAttack()
{
	ActionComponent->StartActionByName(this, "Blackhole");
}


// Dash ability method
void ASCharacter::DashCast()
{
	ActionComponent->StartActionByName(this, "Dash");
}


// Called when inputs primary interact is triggered
void ASCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}


// Called when inputs primary attack is triggered
void ASCharacter::JumpTriggered()
{
	ActionComponent->StartActionByName(this, "Jump");
}


// Called when inputs primary attack is canceled
void ASCharacter::JumpCanceled()
{
	ActionComponent->StopActionByName(this, "Jump");
}


// Character will move faster
void ASCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}


// Character return to normal moving velocity
void ASCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}


// The broadcast function that notifies when Health changes on AttributesComponent
void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	// Damaged
	if (Delta < 0)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(IsHealParamName, false);
	}
		
	// Healed
	if (Delta > 0)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(IsHealParamName, true);
	}

	// TimeToHit act over the flash color when damaged or heal
	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

	// Death if
	if (NewHealth == 0.0f  &&  Delta < 0.0f)
	{
		Dead();
	}
}


// Method called when Mesh begin overlap with another collision actor
void ASCharacter::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult &HitResult)
{
	// Execute interact when it is possible
	if (OtherActor->Implements<USGameplayInterface>())
	{
		ISGameplayInterface::Execute_Interact(OtherActor, this);
	}
}


// Logic when (this) character dies
void ASCharacter::Dead()
{
	// Disable input
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	DisableInput(PlayerController);
	
	// Disable collisions
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement(); // Without this line, the character just falls from anywhere bc collisions are disabled
	
	SetLifeSpan(5.0f);
}



// Override for Pawn method to set the PawnView we want
FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}


// Return Camera forward vector
FVector ASCharacter::GetPawnViewForwardVector() const
{
	return CameraComponent->GetForwardVector();
}


// DEBUG: To quick heal on testing
void ASCharacter::HealSelf(const float Amount /* =1000*/)
{
	AttributesComponent->ApplyHealthChange(this, Amount);
}
