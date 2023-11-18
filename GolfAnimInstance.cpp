// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfAnimInstance.h"

UGolfAnimInstance::UGolfAnimInstance()
{
	Driver = false;
	Hybride = false;
	Iron = false;
	Putter = false;
	Wedge = false;
}

void UGolfAnimInstance::SetDriver()
{
	Driver = true;
}

void UGolfAnimInstance::SetHybride()
{
	Hybride = true;
}

void UGolfAnimInstance::SetIron()
{
	Iron = true;
}

void UGolfAnimInstance::SetPutter()
{
	Putter = true;
}

void UGolfAnimInstance::SetWedge()
{
	Wedge = true;
}
