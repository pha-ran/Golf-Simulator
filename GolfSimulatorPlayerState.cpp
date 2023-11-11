// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfSimulatorPlayerState.h"
#include "Net/UnrealNetwork.h"

AGolfSimulatorPlayerState::AGolfSimulatorPlayerState()
{
	PlayerTurn = 0;
}

void AGolfSimulatorPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGolfSimulatorPlayerState, PlayerTurn);
}

void AGolfSimulatorPlayerState::SetPlayerTurn(int Turn)
{
	if (HasAuthority())
	{
		PlayerTurn = Turn;
	}
}
