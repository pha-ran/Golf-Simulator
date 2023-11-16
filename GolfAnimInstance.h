// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GolfAnimInstance.generated.h"

UCLASS()
class GOLFSIMULATOR_API UGolfAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UGolfAnimInstance();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Animation1;

public:
	void SetAnimation1();

};
