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

	/** ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwingAction;

	/** ���� ��� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PredictAction;

	/** ���� ���� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AngleUpAction;

	/** ���� ���� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AngleDownAction;

	/** ���� �ӵ� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpeedUpAction;

	/** ���� �ӵ� ���� */
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

	/** ���� ������, ������ ��. SpawnProjectile �� �Է¿� ���� ���ε����� �ʰ� �ϴ� ���� */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float SwingRate;

	/** true �� ��� �����ϴ� ���μ��� ���� */
	bool bIsSwung;

	/** ���� ���̿� ���� �����̸� �ִ� Ÿ�̸� �ڵ� */
	FTimerHandle SwingTimer;

	// �ִ� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double MaxAngle;

	// �ּ� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double MinAngle;

	// ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double Angle;

	// ���� ���� ��ȭ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double DeltaAngle;

	// �ִ� ���� �ӵ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float MaxSpeed;

	// �ּ� ���� �ӵ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float MinSpeed;

	// ���� �ӵ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float Speed;

	// ���� �ӵ� ��ȭ�� ����
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

	/** ���� */
	void Swing(const FInputActionValue& Value);

	/** ���� ��� */
	void Predict(const FInputActionValue& Value);

	/** ���� ���� ���� */
	void AngleUp(const FInputActionValue& Value);

	/** ���� ���� ���� */
	void AngleDown(const FInputActionValue& Value);

	/** ���� �ӵ� ���� */
	void SpeedUp(const FInputActionValue& Value);

	/** ���� �ӵ� ���� */
	void SpeedDown(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	/** ���� ���� �Լ� */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartSwing();

	/** ���� ���� �Լ�. ȣ��Ǹ� �÷��̾ StartSwing �� �ٽ� ��� ���� */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopSwing();

	/** ����ü�� �����ϴ� ���� �Լ� */
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
