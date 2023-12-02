// Copyright Epic Games, Inc. All Rights Reserved.

#include "GolfSimulatorGameMode.h"
#include "GolfSimulatorCharacter.h"
#include "GolfSimulatorGameState.h"
#include "GolfSimulatorHUD.h"
#include "GolfSimulatorPlayerController.h"
#include "GolfSimulatorPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AGolfSimulatorGameMode::AGolfSimulatorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_GolfBallCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;

		GameStateClass = AGolfSimulatorGameState::StaticClass();
		HUDClass = AGolfSimulatorHUD::StaticClass();
		PlayerControllerClass = AGolfSimulatorPlayerController::StaticClass();
		PlayerStateClass = AGolfSimulatorPlayerState::StaticClass();
	}

	Turn = 1;
}

void AGolfSimulatorGameMode::BeginPlay()
{
	Super::BeginPlay();

	GolfSimulatorGameState = Cast<AGolfSimulatorGameState>(GameState);
}

void AGolfSimulatorGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AGolfSimulatorPlayerController* GolfSimulatorPlayerController = Cast<AGolfSimulatorPlayerController>(NewPlayer);
	AGolfSimulatorPlayerState* GolfSimulatorPlayerState = Cast<AGolfSimulatorPlayerState>(NewPlayer->PlayerState);

	if (GolfSimulatorPlayerController != nullptr)
	{
		GolfSimulatorPlayerController->Initialize();
	}

	if (GolfSimulatorPlayerState != nullptr)
	{
		GolfSimulatorPlayerState->SetPlayerTurn(Turn);
		Turn++;
	}

	if (GolfSimulatorGameState != nullptr && GolfSimulatorGameState->PlayerArray.Num() == 2 && GolfSimulatorGameState->GetCurrentTurn() == 0)
	{
		GolfSimulatorGameState->NextTurn();
	}
}
