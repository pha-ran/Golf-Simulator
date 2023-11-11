// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GolfSimulatorPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GOLFSIMULATOR_API AGolfSimulatorPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AGolfSimulatorPlayerState();

protected:
	UPROPERTY(Replicated)
	int PlayerTurn;

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE int GetPlayerTurn() const { return PlayerTurn; }
	void SetPlayerTurn(int Turn);
	
};
