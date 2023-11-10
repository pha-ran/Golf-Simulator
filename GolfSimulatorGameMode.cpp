// Copyright Epic Games, Inc. All Rights Reserved.

#include "GolfSimulatorGameMode.h"
#include "GolfSimulatorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGolfSimulatorGameMode::AGolfSimulatorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
