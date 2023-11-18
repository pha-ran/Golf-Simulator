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
	bool Driver;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Hybride;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Iron;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Putter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Wedge;

public:
	void SetDriver();

	void SetHybride();

	void SetIron();

	void SetPutter();

	void SetWedge();

};
