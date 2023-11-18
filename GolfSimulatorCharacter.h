// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GolfSimulatorCharacter.generated.h"


UCLASS(config=Game)
class AGolfSimulatorCharacter : public ACharacter
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

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	AGolfSimulatorCharacter();

protected:
	UStaticMeshComponent* StaticMeshComponent;

	UStaticMesh* HybrideObject;

	UStaticMesh* PutterObject;

	UStaticMesh* IronObject;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

protected:
	void EndTurn();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	UFUNCTION(Server, Reliable)
	void SetDriverAnimation();

	UFUNCTION(Server, Reliable)
	void SetHybrideAnimation();

	UFUNCTION(Server, Reliable)
	void SetIronAnimation();

	UFUNCTION(Server, Reliable)
	void SetPutterAnimation();

	UFUNCTION(Server, Reliable)
	void SetWedgeAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void PlayDriverAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void PlayHybrideAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void PlayIronAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void PlayPutterAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void PlayWedgeAnimation();

};

