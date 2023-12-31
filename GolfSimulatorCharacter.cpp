// Copyright Epic Games, Inc. All Rights Reserved.

#include "GolfSimulatorCharacter.h"
#include "GolfAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"


//////////////////////////////////////////////////////////////////////////
// AGolfSimulatorCharacter

AGolfSimulatorCharacter::AGolfSimulatorCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetSimulatePhysics(false);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GolfClub02(TEXT("/Game/Assets/Golf/Meshes/SM_Golfprops_02"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GolfClub03(TEXT("/Game/Assets/Golf/Meshes/SM_Golfprops_03"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GolfClub04(TEXT("/Game/Assets/Golf/Meshes/SM_Golfprops_04"));

	if (GolfClub02.Succeeded())
	{
		HybrideObject = GolfClub02.Object;
	}

	if (GolfClub03.Succeeded())
	{
		PutterObject = GolfClub03.Object;
	}

	if (GolfClub04.Succeeded())
	{
		IronObject = GolfClub04.Object;
	}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("hand_r")));
}

void AGolfSimulatorCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGolfSimulatorCharacter::EndTurn()
{
	if (HasAuthority())
	{
		Destroy();
	}
}

void AGolfSimulatorCharacter::SetDriverAnimation_Implementation()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGolfSimulatorCharacter::EndTurn, 4.0f, false);

	PlayDriverAnimation();
}

void AGolfSimulatorCharacter::SetHybrideAnimation_Implementation()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGolfSimulatorCharacter::EndTurn, 4.0f, false);

	PlayHybrideAnimation();
}

void AGolfSimulatorCharacter::SetIronAnimation_Implementation()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGolfSimulatorCharacter::EndTurn, 4.0f, false);

	PlayIronAnimation();
}

void AGolfSimulatorCharacter::SetPutterAnimation_Implementation()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGolfSimulatorCharacter::EndTurn, 3.0f, false);

	PlayPutterAnimation();
}

void AGolfSimulatorCharacter::SetWedgeAnimation_Implementation()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGolfSimulatorCharacter::EndTurn, 11.0f, false);

	PlayWedgeAnimation();
}

void AGolfSimulatorCharacter::PlayDriverAnimation_Implementation()
{
	if (HybrideObject != nullptr && StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(HybrideObject);
		StaticMeshComponent->SetRelativeLocationAndRotation(FVector(-9.47789, 0.839751, 5.931521), FRotator(-12.952066, -82.371366, 149.13423));
	}

	UGolfAnimInstance* GolfAnimInstance = Cast<UGolfAnimInstance>(GetMesh()->GetAnimInstance());

	if (GolfAnimInstance != nullptr)
	{
		GolfAnimInstance->SetDriver();
	}
}

void AGolfSimulatorCharacter::PlayHybrideAnimation_Implementation()
{
	if (HybrideObject != nullptr && StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(HybrideObject);
		StaticMeshComponent->SetRelativeLocationAndRotation(FVector(-10.997305, 0.899521, 4.663407), FRotator(-8.648474, -84.965307, 139.621413));
	}

	UGolfAnimInstance* GolfAnimInstance = Cast<UGolfAnimInstance>(GetMesh()->GetAnimInstance());

	if (GolfAnimInstance != nullptr)
	{
		GolfAnimInstance->SetHybride();
	}
}

void AGolfSimulatorCharacter::PlayIronAnimation_Implementation()
{
	if (IronObject != nullptr && StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(IronObject);
		StaticMeshComponent->SetRelativeLocationAndRotation(FVector(4.112263, 1.863093, -12.531529), FRotator(-62.241651, -141.779043, 152.125056));
	}

	UGolfAnimInstance* GolfAnimInstance = Cast<UGolfAnimInstance>(GetMesh()->GetAnimInstance());

	if (GolfAnimInstance != nullptr)
	{
		GolfAnimInstance->SetIron();
	}
}

void AGolfSimulatorCharacter::PlayPutterAnimation_Implementation()
{
	if (PutterObject != nullptr && StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(PutterObject);
		StaticMeshComponent->SetRelativeLocationAndRotation(FVector(-1.778892, 6.11454, -5.067813), FRotator(-57.525185, -176.650659, -147.746515));
	}

	UGolfAnimInstance* GolfAnimInstance = Cast<UGolfAnimInstance>(GetMesh()->GetAnimInstance());

	if (GolfAnimInstance != nullptr)
	{
		GolfAnimInstance->SetPutter();
	}
}

void AGolfSimulatorCharacter::PlayWedgeAnimation_Implementation()
{
	if (IronObject != nullptr && StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(IronObject);
		StaticMeshComponent->SetRelativeLocationAndRotation(FVector(2.680225, 5.119117, -4.531633), FRotator(-50.567415, 123.903952, -125.195479));
	}

	UGolfAnimInstance* GolfAnimInstance = Cast<UGolfAnimInstance>(GetMesh()->GetAnimInstance());

	if (GolfAnimInstance != nullptr)
	{
		GolfAnimInstance->SetWedge();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGolfSimulatorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGolfSimulatorCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGolfSimulatorCharacter::Look);

	}

}

void AGolfSimulatorCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGolfSimulatorCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
