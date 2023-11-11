// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GolfSimulatorGameState.h"
#include "GameFramework/GameModeBase.h"
#include "GolfSimulatorGameMode.generated.h"

UCLASS(minimalapi)
class AGolfSimulatorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGolfSimulatorGameMode();

protected:
	AGolfSimulatorGameState* GolfSimulatorGameState;

	int Turn;

protected:
	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;

};



