// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfSimulatorPlayerController.h"
#include "InGameUserWidget.h"
#include "UObject/ConstructorHelpers.h"

AGolfSimulatorPlayerController::AGolfSimulatorPlayerController()
{
	static ConstructorHelpers::FClassFinder<UInGameUserWidget> _InGameUserWidgetBlueprint(TEXT("/Game/ThirdPerson/Blueprints/BP_InGameUserWidget"));

	if (_InGameUserWidgetBlueprint.Class != NULL)
	{
		InGameUserWidgetBlueprint = _InGameUserWidgetBlueprint.Class;
	}
}

void AGolfSimulatorPlayerController::BeginPlay()
{
	if (!HasAuthority())
	{
		if (InGameUserWidgetBlueprint != nullptr)
		{
			InGameUserWidget = CreateWidget<UInGameUserWidget>(this, InGameUserWidgetBlueprint);

			InGameUserWidget->AddToViewport();
		}
	}
}

void AGolfSimulatorPlayerController::Tick(float DeltaTime)
{
	;
}
