// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Components/InputComponent.h"
#include "PlayerPawn.generated.h"

class UEnhancedInputComponent;
class ABullet;

UCLASS()
class GROUP_7_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

			/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;

	/** Refrence to Bullet BP*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		TSubclassOf<ABullet> BP_Bullet;

	/** Public Variables */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int AmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int GranadeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int MaxGranade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int Lives;

			/** Input */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* ForwardBackwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* RightLeftInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* ShootInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* ReloadInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* ThrowInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* MouseXInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* MouseYInput;

private:
		/** Private Functions */

	UFUNCTION(BlueprintCallable)
	void ForwardBackward(const FInputActionValue& input);

	UFUNCTION(BlueprintCallable)
	void RightLeft(const FInputActionValue& input);

	UFUNCTION(BlueprintCallable)
	void Shoot(const FInputActionValue& input);

	UFUNCTION(BlueprintCallable)
	void Reload(const FInputActionValue& input);

	UFUNCTION(BlueprintCallable)
	void Throw(const FInputActionValue& input);

	UFUNCTION(BlueprintCallable)
	void MouseX(const FInputActionValue& input);

	UFUNCTION(BlueprintCallable)
	void MouseY(const FInputActionValue& input);

			/** Private Vals */
private:
	float XInput;
	float YInput;

	float Yaw;
	float Pitch;
	FVector ForwardVector;

};
