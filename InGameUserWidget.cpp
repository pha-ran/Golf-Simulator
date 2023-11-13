// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUserWidget.h"
#include "Components/TextBlock.h"

void UInGameUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerNameText")));

	Turn = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurnText")));

	Distance = Cast<UTextBlock>(GetWidgetFromName(TEXT("DistanceText")));

	Speed = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpeedText")));

	Angle = Cast<UTextBlock>(GetWidgetFromName(TEXT("AngleText")));
}

void UInGameUserWidget::SetPlayerNameText(int _PlayerTurn)
{
	if (PlayerName != nullptr)
	{
		FString PlayerNameString = FString::Printf(TEXT("Player %d"), _PlayerTurn);
		FText PlayerNameText = FText::FromString(PlayerNameString);
		PlayerName->SetText(PlayerNameText);
	}
}

void UInGameUserWidget::SetTurnText(int _Turn)
{
	if (Turn != nullptr)
	{
		FString TurnString = FString::Printf(TEXT("Player%d Turn"), _Turn);
		FText TurnText = FText::FromString(TurnString);
		Turn->SetText(TurnText);
	}
}

void UInGameUserWidget::SetDistanceText(double _Distance)
{
	if (Distance != nullptr)
	{
		FString DistanceString = FString::Printf(TEXT("%.1f M"), _Distance / 100);
		FText DistanceText = FText::FromString(DistanceString);
		Distance->SetText(DistanceText);
	}
}

void UInGameUserWidget::SetSpeedText(float _Speed)
{
	if (Speed != nullptr)
	{
		FString SpeedString = FString::Printf(TEXT("%.0f"), _Speed / 100);
		FText SpeedText = FText::FromString(SpeedString);
		Speed->SetText(SpeedText);
	}
}

void UInGameUserWidget::SetAngleText(double _Angle)
{
	if (Angle != nullptr)
	{
		FString AngleString = FString::Printf(TEXT("%.0f"), _Angle);
		FText AngleText = FText::FromString(AngleString);
		Angle->SetText(AngleText);
	}
}
