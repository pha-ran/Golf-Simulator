// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfBallCharacter.h"
#include "GolfBallProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGolfBallCharacter::AGolfBallCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;

	if (DefaultMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultMesh.Object);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -2.5f));
		StaticMesh->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
	}

	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	static ConstructorHelpers::FClassFinder<APawn> GolfBallProjectileBP(TEXT("/Game/ThirdPerson/Blueprints/BP_GolfBallProjectile"));

	if (GolfBallProjectileBP.Class != NULL)
	{
		GolfBallProjectileBPClass = GolfBallProjectileBP.Class;
	}

	SwingRate = 1.0f;
	bIsSwung = false;

	MaxAngle = 75.0f;
	MinAngle = 0.0f;
	Angle = 0.0f;
	DeltaAngle = 2.5f;

	MaxSpeed = 6000.0f;
	MinSpeed = 100.0f;
	Speed = 0.0f;
	DeltaSpeed = 100.0f;

	bSwingIgnore = true;

	NextLocationX = 1000.0f;
	NextLocationY = 500.0f;
	NextLocationZ = 0.0f;

	SetActorHiddenInGame(true);
}

void AGolfBallCharacter::Look(const FInputActionValue& Value)
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

void AGolfBallCharacter::Swing(const FInputActionValue& Value)
{
	if (bSwingIgnore)
	{
		return;
	}

	bool IsPressed = Value.Get<bool>();

	if (IsPressed)
	{
		StartSwing();

		UE_LOG(LogTemp, Display, TEXT("Swing"));
	}
}

void AGolfBallCharacter::Predict(const FInputActionValue& Value)
{
	if (bSwingIgnore)
	{
		return;
	}

	bool IsPressed = Value.Get<bool>();

	if (IsPressed)
	{
		UE_LOG(LogTemp, Display, TEXT("Predict"));
	}
}

void AGolfBallCharacter::AngleUp(const FInputActionValue& Value)
{
	if (bSwingIgnore)
	{
		return;
	}

	bool IsPressed = Value.Get<bool>();

	if (IsPressed && Angle <= (MaxAngle - DeltaAngle))
	{
		Angle += DeltaAngle;

		UE_LOG(LogTemp, Display, TEXT("AngleUp %f"), Angle);
	}
}

void AGolfBallCharacter::AngleDown(const FInputActionValue& Value)
{
	if (bSwingIgnore)
	{
		return;
	}

	bool IsPressed = Value.Get<bool>();

	if (IsPressed && Angle >= (MinAngle + DeltaAngle))
	{
		Angle -= DeltaAngle;

		UE_LOG(LogTemp, Display, TEXT("AngleDown %f"), Angle);
	}
}

void AGolfBallCharacter::SpeedUp(const FInputActionValue& Value)
{
	if (bSwingIgnore)
	{
		return;
	}

	bool IsPressed = Value.Get<bool>();

	if (IsPressed && Speed <= (MaxSpeed - DeltaSpeed))
	{
		Speed += DeltaSpeed;

		UE_LOG(LogTemp, Display, TEXT("SpeedUp %f"), Speed);
	}
}

void AGolfBallCharacter::SpeedDown(const FInputActionValue& Value)
{
	if (bSwingIgnore)
	{
		return;
	}

	bool IsPressed = Value.Get<bool>();

	if (IsPressed && Speed >= (MinSpeed + DeltaSpeed))
	{
		Speed -= DeltaSpeed;

		UE_LOG(LogTemp, Display, TEXT("SpeedDown %f"), Speed);
	}
}

// Called when the game starts or when spawned
void AGolfBallCharacter::BeginPlay()
{
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

void AGolfBallCharacter::StartSwing()
{
	if (!bIsSwung)
	{
		bSwingIgnore = true;
		bIsSwung = true;

		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(SwingTimer, this, &AGolfBallCharacter::StopSwing, SwingRate, false);

		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		SpawnProjectile(CameraRotation, Angle, Speed);
	}
}

void AGolfBallCharacter::StopSwing()
{
	bIsSwung = false;
}

void AGolfBallCharacter::SpawnProjectile_Implementation(FRotator _CameraRotation, double _Angle, float _Speed)
{
	FVector SpawnLocation = GetActorLocation() + FTransform(_CameraRotation).TransformVector(FVector(0.0f, 0.0f, 0.0f));
	FRotator SpawnRotation = _CameraRotation;

	SpawnRotation.Pitch += _Angle;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = this;

	AGolfBallProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AGolfBallProjectile>(GolfBallProjectileBPClass, SpawnLocation, SpawnRotation, SpawnParameters);

	if (SpawnedProjectile)
	{
		FVector Direction = SpawnRotation.Vector();
		SpawnedProjectile->Setup(Direction, _Speed);
		
		SetActorHiddenInGame(true);
	}
}

// Called every frame
void AGolfBallCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGolfBallCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGolfBallCharacter, bSwingIgnore);

	DOREPLIFETIME(AGolfBallCharacter, NextLocationX);
	DOREPLIFETIME(AGolfBallCharacter, NextLocationY);
	DOREPLIFETIME(AGolfBallCharacter, NextLocationZ);
}

void AGolfBallCharacter::SetSwingIgnore(bool Ignore)
{
	bSwingIgnore = Ignore;
}

void AGolfBallCharacter::SetNextLocation(FVector _Location)
{
	if (HasAuthority())
	{
		NextLocationX = _Location.X;
		NextLocationY = _Location.Y;
		NextLocationZ = _Location.Z;
	}
}

void AGolfBallCharacter::MoveNextLocation_Implementation(FVector _Location)
{
	SetActorLocation(_Location);
	SetActorHiddenInGame(false);
}

// Called to bind functionality to input
void AGolfBallCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGolfBallCharacter::Look);

		//스윙
		EnhancedInputComponent->BindAction(SwingAction, ETriggerEvent::Triggered, this, &AGolfBallCharacter::Swing);

		//예상 경로
		EnhancedInputComponent->BindAction(PredictAction, ETriggerEvent::Triggered, this, &AGolfBallCharacter::Predict);

		//세로 각도 증가
		EnhancedInputComponent->BindAction(AngleUpAction, ETriggerEvent::Triggered, this, &AGolfBallCharacter::AngleUp);

		//세로 각도 감소
		EnhancedInputComponent->BindAction(AngleDownAction, ETriggerEvent::Triggered, this, &AGolfBallCharacter::AngleDown);

		//스윙 속도 증가
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Triggered, this, &AGolfBallCharacter::SpeedUp);

		//스윙 속도 감소
		EnhancedInputComponent->BindAction(SpeedDownAction, ETriggerEvent::Triggered, this, &AGolfBallCharacter::SpeedDown);
	}
}

