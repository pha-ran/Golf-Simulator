// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfSimulatorPlayerController.h"
#include "GolfBallCharacter.h"
#include "GolfSimulatorGameState.h"
#include "GolfSimulatorPlayerState.h"
#include "InGameUserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

AGolfSimulatorPlayerController::AGolfSimulatorPlayerController()
{
	static ConstructorHelpers::FClassFinder<UInGameUserWidget> _InGameUserWidgetBlueprint(TEXT("/Game/ThirdPerson/Blueprints/BP_InGameUserWidget"));

	if (_InGameUserWidgetBlueprint.Class != NULL)
	{
		InGameUserWidgetBlueprint = _InGameUserWidgetBlueprint.Class;
	}
}

void AGolfSimulatorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
}

void AGolfSimulatorPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
	{
		if (GolfBallCharacter == nullptr)
		{
			GolfBallCharacter = Cast<AGolfBallCharacter>(GetPawn());
		}

		if (GolfSimulatorGameState == nullptr)
		{
			GolfSimulatorGameState = Cast<AGolfSimulatorGameState>(GetWorld()->GetGameState());
		}

		if (GolfSimulatorPlayerState == nullptr)
		{
			GolfSimulatorPlayerState = Cast<AGolfSimulatorPlayerState>(PlayerState);
		}
	}

	if (!HasAuthority() && GolfBallCharacter != nullptr && GolfSimulatorGameState != nullptr && GolfSimulatorPlayerState != nullptr && InGameUserWidget != nullptr)
	{
		InGameUserWidget->SetPlayerNameText(GolfSimulatorPlayerState->GetPlayerTurn());

		InGameUserWidget->SetTurnText(GolfSimulatorGameState->GetCurrentTurn());

		InGameUserWidget->SetDistanceText(FVector::Distance(GolfBallCharacter->GetActorLocation(), FVector(11060.0f, 2930.0f, 432.0f)));

		InGameUserWidget->SetSpeedText(GolfBallCharacter->GetSpeed());

		InGameUserWidget->SetAngleText(GolfBallCharacter->GetAngle());
	}
}

void AGolfSimulatorPlayerController::Initialize_Implementation()
{
	if (!HasAuthority())
	{
		if (InGameUserWidgetBlueprint != nullptr)
		{
			InGameUserWidget = CreateWidget<UInGameUserWidget>(this, InGameUserWidgetBlueprint);

			InGameUserWidget->AddToViewport();
		}
	}
}
