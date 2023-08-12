// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up Interaction Interface Component
	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");

	// Set up Attributes Component
	AttributesComponent = CreateDefaultSubobject<USAttributesComponent>("AttributesComponent");

	// Set up CameraComponent (So, SpringArmComponent too)
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent"); // SpringArm
	CameraComponent    = CreateDefaultSubobject<UCameraComponent>("CameraComponent");		// Camera
	SpringArmComponent->SetupAttachment(RootComponent);									// Attachment SpringArm->RootComponent
	CameraComponent->SetupAttachment(SpringArmComponent);									// Attachment Camera->SpringArm
	SpringArmComponent->bUsePawnControlRotation = true;			// Some constructor values  /-
	bUseControllerRotationYaw = false;							// Some constructor values { https://drive.google.com/file/d/1QDxeYIUHOry3bJtOwmN2_SAaEdPWIXTy/view?usp=sharing
	GetCharacterMovement()->bOrientRotationToMovement = true;	// Some constructor values  \_
	HitAttackRange = 10000.0f;                                  // Some Constructor values
	HandSocketName = "Muzzle_01";

	// Activate OverlapEvents on player mesh
	GetMesh()->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set up Input
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext,0);
		}
	}
	
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

	/* DEBUG TO SEE WHAT IS DOING ON REAL GAMEPLAY THIS VARIABLES
	if (GetLocalViewingPlayerController()->WasInputKeyJustPressed(EKeys::One))
		SpringArmComponent->bUsePawnControlRotation = !SpringArmComponent->bUsePawnControlRotation;
		GEngine->AddOnScreenDebugMessage(-1,4,FColor::Yellow, FString::Printf(TEXT("bUseControllerRotationYaw to %s"), (SpringArmComponent->bUsePawnControlRotation) ? TEXT("true") : TEXT("false")));
	}

	if (GetLocalViewingPlayerController()->WasInputKeyJustPressed(EKeys::Two))
		bUseControllerRotationYaw = !bUseControllerRotationYaw;
		GEngine->AddOnScreenDebugMessage(-1,4,FColor::Yellow, FString::Printf(TEXT("bUseControllerRotationYaw to %s"), (bUseControllerRotationYaw) ? TEXT("true") : TEXT("false")));
	}

	if (GetLocalViewingPlayerController()->WasInputKeyJustPressed(EKeys::Three)) {
		GetCharacterMovement()->bOrientRotationToMovement = !GetCharacterMovement()->bOrientRotationToMovement;
		GEngine->AddOnScreenDebugMessage(-1,4,FColor::Yellow, FString::Printf(TEXT("bUseControllerRotationYaw to %s"), (GetCharacterMovement()->bOrientRotationToMovement) ? TEXT("true") : TEXT("false")));
	}
	*/

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	const FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	const FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,            ETriggerEvent::Triggered, this, &ASCharacter::Move           );
		EnhancedInputComponent->BindAction(LookAction,            ETriggerEvent::Triggered, this, &ASCharacter::Look           );
		EnhancedInputComponent->BindAction(PrimaryAttackAction,   ETriggerEvent::Started,   this, &ASCharacter::PrimaryAttack  );
		EnhancedInputComponent->BindAction(SecondaryAttackAction, ETriggerEvent::Started,   this, &ASCharacter::SecondaryAttack);
		EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started,   this, &ASCharacter::PrimaryInteract);
		EnhancedInputComponent->BindAction(JumpAction,            ETriggerEvent::Started,   this, &ASCharacter::JumpTriggered  );		
		EnhancedInputComponent->BindAction(JumpAction,            ETriggerEvent::Canceled,  this, &ASCharacter::JumpCanceled   );		
		EnhancedInputComponent->BindAction(DashAction,            ETriggerEvent::Started,   this, &ASCharacter::DashCast       );
	}
}


// Getter for Mesh
USkeletalMeshComponent* ASCharacter::GetSKMesh() const
{
	return GetMesh();
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
	if (!IsAnyAttackTimerPending())
	{
		PlayAnimMontage(AttackAnim);
		
		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.17f);
	}
}


// Secondary attack method
void ASCharacter::SecondaryAttack()
{
	if (!IsAnyAttackTimerPending())
	{
		PlayAnimMontage(AttackAnim);
		
		GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, 0.2f);
	}
}


// Dash ability method
void ASCharacter::DashCast()
{
	if (!IsAnyAttackTimerPending())
	{
		PlayAnimMontage(AttackAnim);
		
		GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ASCharacter::DashAbility_TimeElapsed, 0.2f);
	}
}


// Called when TimerHandle_PrimaryAttack time is elapsed 
void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(PrimaryProjectileClass);
}


// Called when TimerHandle_SecondaryAttack time is elapsed 
void ASCharacter::SecondaryAttack_TimeElapsed()
{
	SpawnProjectile(SecondaryProjectileClass);
}


// Called when TimerHandle_Dash time is elapsed 
void ASCharacter::DashAbility_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}


// Called to spawn projectiles from player
void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	// Getting some parameters for sweep raycast projectile trajectory searching hit points 
	const FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
	FHitResult HitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	// To ignore Player
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);	

	// Calculation of the projectile trajectory and possible hit point
	const FVector SweepStartPoint = CameraComponent->GetComponentLocation();
	const FVector SweepEndPoint   = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector()*HitAttackRange;
	const bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult, SweepStartPoint, SweepEndPoint, ObjectQueryParams, Params);
	
	// Calculation of the projectile spawn rotation from the hand (HandLocation to ProjectileEndLocation)
	const FVector ProjectileEndLocation = bBlockingHit ? HitResult.Location : SweepEndPoint;
	const FRotator ProjectileRotation = (ProjectileEndLocation - HandLocation).Rotation();

	// Setting some parameters for projectile spawn
	const FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	// Projectile Spawn
	GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);

	// DEBUG: Draw sphere on ProjectileEndLocation
	DrawDebugSphere(GetWorld(),ProjectileEndLocation,32.0f,32.f,FColor::Black,false,3.0f);
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
	Jump();
}


// Called when inputs primary attack is canceled
void ASCharacter::JumpCanceled()
{
	StopJumping();
}


// The broadcast function that notifies when Health changes on AttributesComponent
void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	// Damaged
	if (Delta < 0)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("IsHeal", false);
	}
		
	// Healed
	if (Delta > 0)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("IsHeal", true);
	}

	// TimeToHit act over the flash color when damaged or heal
	GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

	// Death if
	if (NewHealth == 0.0f  &&  Delta < 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
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


// Return true if any attack timer handle is active
bool ASCharacter::IsAnyAttackTimerPending()
{
	const bool IsPrimaryAttackTimerActive   = GetWorldTimerManager().IsTimerActive(TimerHandle_PrimaryAttack  );
	const bool IsSecondaryAttackTimerActive = GetWorldTimerManager().IsTimerActive(TimerHandle_SecondaryAttack);
	const bool IsDashTimerActive            = GetWorldTimerManager().IsTimerActive(TimerHandle_Dash           );

		
	return (IsPrimaryAttackTimerActive  ||  IsSecondaryAttackTimerActive  ||  IsDashTimerActive);
}
