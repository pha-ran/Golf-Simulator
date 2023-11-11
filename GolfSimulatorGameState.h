// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GolfSimulatorGameState.generated.h"

/**
 * 
 */
UCLASS()
class GOLFSIMULATOR_API AGolfSimulatorGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AGolfSimulatorGameState();

protected:
	UPROPERTY(Replicated)
	int CurrentTurn;

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE int GetCurrentTurn() const { return CurrentTurn; }

	void NextTurn();
	
};
