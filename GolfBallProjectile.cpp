// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfBallProjectile.h"
#include "GolfBallCharacter.h"
#include "GolfSimulatorGameState.h"
#include "GolfSimulatorPlayerController.h"
#include "GolfSimulatorPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGolfBallProjectile::AGolfBallProjectile()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(2.5f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SphereComponent->SetIsReplicated(true);
	RootComponent = SphereComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	if (DefaultMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultMesh.Object);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -2.5f));
		StaticMesh->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
	}

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

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->SetIsReplicated(true);

	if (HasAuthority())
	{
		ProjectileMovementComponent->InitialSpeed = 100.0f;
		ProjectileMovementComponent->MaxSpeed = 4000.0f;
		ProjectileMovementComponent->bSimulationEnabled = true;
		ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.125;
		ProjectileMovementComponent->Friction = 0.75f;
		ProjectileMovementComponent->BounceVelocityStopSimulatingThreshold = 0.0f;
	}

	IsStop = false;
	InHole = false;
}

// Called when the game starts or when spawned
void AGolfBallProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority())
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController != nullptr)
		{
			FRotator Rotator = GetActorRotation();
			Rotator.Pitch = -30.0f;
			PlayerController->SetControlRotation(Rotator);
			PlayerController->SetViewTargetWithBlend(this, 0.5f, EViewTargetBlendFunction::VTBlend_Linear, 1.0f, false);
		}
	}
}

void AGolfBallProjectile::EndTurn()
{
	AGolfSimulatorGameState* GolfSimulatorGameState = Cast<AGolfSimulatorGameState>(GetWorld()->GetGameState());

	if (GolfSimulatorGameState != nullptr)
	{
		GolfSimulatorGameState->NextTurn();

		Destroy();
	}
}

// Called every frame
void AGolfBallProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && !IsStop && !InHole && ProjectileMovementComponent->Velocity.IsNearlyZero())
	{
		AGolfBallCharacter* GolfBallCharacter = Cast<AGolfBallCharacter>(GetOwner());

		if (GolfBallCharacter != nullptr)
		{
			IsStop = true;

			GolfBallCharacter->SetNextLocation(GetActorLocation());
			
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AGolfBallProjectile::EndTurn, 3.0f, false);
		}
	}

	if (!HasAuthority())
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController != nullptr)
		{
			FRotator PlayerRotation = PlayerController->GetControlRotation();
			FVector TargetLocation = GetActorLocation();
			float DistanceToMaintain = 400.0f;

			FVector CameraLocation = TargetLocation - PlayerRotation.Vector() * DistanceToMaintain;
			FRotator CameraRotation = (TargetLocation - CameraLocation).Rotation();

			FollowCamera->SetWorldLocationAndRotation(CameraLocation, CameraRotation);
		}
	}
}

// Called to bind functionality to input
void AGolfBallProjectile::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGolfBallProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGolfBallProjectile, IsStop);
	DOREPLIFETIME(AGolfBallProjectile, InHole);
}

void AGolfBallProjectile::Setup(FVector& Direction, float Speed)
{
	ProjectileMovementComponent->Velocity = Direction * (ProjectileMovementComponent->InitialSpeed + Speed);
}

void AGolfBallProjectile::OnHoleBeginOverlap()
{
	InHole = true;

	AGolfBallCharacter* GolfBallCharacter = Cast<AGolfBallCharacter>(GetOwner());

	if (GolfBallCharacter != nullptr)
	{
		AGolfSimulatorPlayerController* PlayerController = GolfBallCharacter->GetController<AGolfSimulatorPlayerController>();

		if (PlayerController != nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("OnHoleBeginOverlap Player %s"), *(PlayerController->PlayerState->GetPlayerName()));
			
			EndTurn();
		}
	}
}
