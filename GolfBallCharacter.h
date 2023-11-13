// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "GolfBallCharacter.generated.h"

UCLASS()
class GOLFSIMULATOR_API AGolfBallCharacter : public APawn
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** 스윙 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwingAction;

	/** 예상 경로 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PredictAction;

	/** 세로 각도 증가 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AngleUpAction;

	/** 세로 각도 감소 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AngleDownAction;

	/** 스윙 속도 증가 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpeedUpAction;

	/** 스윙 속도 감소 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpeedDownAction;

public:
	// Sets default values for this pawn's properties
	AGolfBallCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMesh;

	TSubclassOf<APawn> GolfBallProjectileBPClass;

	/** 스윙 딜레이, 단위는 초. SpawnProjectile 을 입력에 직접 바인딩하지 않게 하는 역할 */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float SwingRate;

	/** true 인 경우 스윙하는 프로세스 도중 */
	bool bIsSwung;

	/** 스폰 사이에 스윙 딜레이를 넣는 타이머 핸들 */
	FTimerHandle SwingTimer;

	// 최대 세로 각도 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double MaxAngle;

	// 최소 세로 각도 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double MinAngle;

	// 세로 각도 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double Angle;

	// 세로 각도 변화량 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double DeltaAngle;

	// 최대 스윙 속도 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float MaxSpeed;

	// 최소 스윙 속도 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float MinSpeed;

	// 스윙 속도 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float Speed;

	// 스윙 속도 변화량 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float DeltaSpeed;

	UPROPERTY(Replicated)
	bool bSwingIgnore;

	UPROPERTY(Replicated)
	double NextLocationX;

	UPROPERTY(Replicated)
	double NextLocationY;

	UPROPERTY(Replicated)
	double NextLocationZ;

protected:
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** 스윙 */
	void Swing(const FInputActionValue& Value);

	/** 예상 경로 */
	void Predict(const FInputActionValue& Value);

	/** 세로 각도 증가 */
	void AngleUp(const FInputActionValue& Value);

	/** 세로 각도 감소 */
	void AngleDown(const FInputActionValue& Value);

	/** 스윙 속도 증가 */
	void SpeedUp(const FInputActionValue& Value);

	/** 스윙 속도 감소 */
	void SpeedDown(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	/** 스윙 시작 함수 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartSwing();

	/** 스윙 종료 함수. 호출되면 플레이어가 StartSwing 을 다시 사용 가능 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopSwing();

	/** 투사체를 스폰하는 서버 함수 */
	UFUNCTION(Server, Reliable)
	void SpawnProjectile(FRotator _CameraRotation, double _Angle, float _Speed);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void SetSwingIgnore(bool Ignore);

	FORCEINLINE FVector GetNextLocation() const { return FVector(NextLocationX, NextLocationY, NextLocationZ); }

	void SetNextLocation(FVector _Location);

	UFUNCTION(NetMulticast, Reliable)
	void MoveNextLocation(FVector _Location);

	FORCEINLINE float GetSpeed() const { return Speed; }

	FORCEINLINE double GetAngle() const { return Angle; }

};
