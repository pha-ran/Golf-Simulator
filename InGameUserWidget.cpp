// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUserWidget.h"
#include "Components/TextBlock.h"

void UInGameUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerNameText")));
}

void UInGameUserWidget::SetPlayerNameText(FString _Name)
{
	if (PlayerName != nullptr)
	{
		PlayerName->SetText(FText::FromString(_Name));
	}
}
