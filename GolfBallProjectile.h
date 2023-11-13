// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"
#include "GolfBallProjectile.generated.h"

UCLASS()
class GOLFSIMULATOR_API AGolfBallProjectile : public APawn
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	// Sets default values for this pawn's properties
	AGolfBallProjectile();

protected:
	// 콜리전 테스트에 사용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	// 오브젝트 비주얼 표현 제공
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMesh;

	// 투사체 움직임 처리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraSystem* NiagaraSystemAssetObject;

	UPROPERTY(Replicated)
	bool IsStop;

	UPROPERTY(Replicated)
	bool InHole;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	void EndTurn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void Setup(FVector& Direction, float Speed);

	void OnHoleBeginOverlap();
};
