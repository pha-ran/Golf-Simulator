// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfSimulatorGameState.h"
#include "GolfBallCharacter.h"
#include "GolfSimulatorPlayerState.h"
#include "Net/UnrealNetwork.h"

AGolfSimulatorGameState::AGolfSimulatorGameState()
{
	CurrentTurn = 0;
}

void AGolfSimulatorGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGolfSimulatorGameState, CurrentTurn);
}

void AGolfSimulatorGameState::NextTurn()
{
	if (!HasAuthority())
	{
		return;
	}
	
	for (APlayerState* PlayerState : PlayerArray)
	{
		AGolfBallCharacter* GolfBallCharacter = Cast<AGolfBallCharacter>(PlayerState->GetPawn());

		if (GolfBallCharacter != nullptr)
		{
			GolfBallCharacter->SetSwingIgnore(true);
		}
	}

	for (APlayerState* PlayerState : PlayerArray)
	{
		AGolfBallCharacter* GolfBallCharacter = Cast<AGolfBallCharacter>(PlayerState->GetPawn());
		AGolfSimulatorPlayerState* GolfSimulatorPlayerState = Cast<AGolfSimulatorPlayerState>(PlayerState);

		if (GolfBallCharacter != nullptr && GolfSimulatorPlayerState != nullptr)
		{
			if (CurrentTurn < GolfSimulatorPlayerState->GetPlayerTurn())
			{
				CurrentTurn = GolfSimulatorPlayerState->GetPlayerTurn();
				GolfBallCharacter->SetSwingIgnore(false);
				UE_LOG(LogTemp, Display, TEXT("NextTurn %d"), CurrentTurn);
				return;
			}
		}
	}

	AGolfBallCharacter* GolfBallCharacter = Cast<AGolfBallCharacter>(PlayerArray[0]->GetPawn());
	AGolfSimulatorPlayerState* GolfSimulatorPlayerState = Cast<AGolfSimulatorPlayerState>(PlayerArray[0]);

	if (GolfBallCharacter != nullptr && GolfSimulatorPlayerState != nullptr)
	{
		CurrentTurn = GolfSimulatorPlayerState->GetPlayerTurn();
		GolfBallCharacter->SetSwingIgnore(false);
		UE_LOG(LogTemp, Display, TEXT("NextTurn %d"), CurrentTurn);
	}
}
