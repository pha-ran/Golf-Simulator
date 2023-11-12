// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUserWidget.generated.h"

UCLASS()
class GOLFSIMULATOR_API UInGameUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* PlayerName;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetPlayerNameText(FString _Name);
	
};
