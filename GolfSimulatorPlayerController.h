// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GolfBallCharacter.h"
#include "GolfSimulatorGameState.h"
#include "GolfSimulatorPlayerState.h"
#include "GolfSimulatorPlayerController.generated.h"

UCLASS()
class GOLFSIMULATOR_API AGolfSimulatorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGolfSimulatorPlayerController();

protected:
	TSubclassOf<class UInGameUserWidget> InGameUserWidgetBlueprint;

	class UInGameUserWidget* InGameUserWidget;

	AGolfBallCharacter* GolfBallCharacter;

	AGolfSimulatorGameState* GolfSimulatorGameState;

	AGolfSimulatorPlayerState* GolfSimulatorPlayerState;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(Client, Reliable)
	void Initialize();

};
