// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfBallCharacter.h"

// Sets default values
AGolfBallCharacter::AGolfBallCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGolfBallCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGolfBallCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGolfBallCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

