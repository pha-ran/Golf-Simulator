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

void AGolfSimulatorGameState::NextTurn_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}

	bool FinishAll = true;

	for (APlayerState* PlayerState : PlayerArray)
	{
		AGolfBallCharacter* GolfBallCharacter = Cast<AGolfBallCharacter>(PlayerState->GetPawn());

		if (GolfBallCharacter != nullptr)
		{
			if (!GolfBallCharacter->GetFinish())
			{
				FinishAll = false;
			}
		}
	}

	if (FinishAll)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			World->ServerTravel("/Game/Maps/GolfHouseMap/House01_F1_H02", true, false);
		}
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
				FVector NextLocation = GolfBallCharacter->GetNextLocation();

				if (NextLocation.X == 0.0f && NextLocation.Y == 0.0f && NextLocation.Z == 0.0f)
				{
					UWorld* World = GetWorld();
					FString CurrentMapName = World->GetMapName();

					if (CurrentMapName.Compare("C001_H01") == 0)
					{
						GolfBallCharacter->SetNextLocation(FVector(-11710.0f, 14130.0f, 338.236f));
					}
					else
					{
						GolfBallCharacter->SetNextLocation(FVector(-4450.003527f, -1329.982854f, 1915.496842f));
					}
				}

				CurrentTurn = GolfSimulatorPlayerState->GetPlayerTurn();
				GolfBallCharacter->MoveNextLocation(GolfBallCharacter->GetNextLocation());
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
		GolfBallCharacter->MoveNextLocation(GolfBallCharacter->GetNextLocation());
		GolfBallCharacter->SetSwingIgnore(false);
		UE_LOG(LogTemp, Display, TEXT("NextTurn %d"), CurrentTurn);
	}
}
