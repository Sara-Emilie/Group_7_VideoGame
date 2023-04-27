// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/InputComponent.h"
#include "MainCharacter.generated.h"


class UEnhancedInputComponent;
class ABullet;
class AGrenade;
class UNiagaraSystem;
class USoundBase;


UCLASS()
class GROUP_7_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AGrenade* Grenade;
	ABullet* Bullet;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class SceneCaptureComponent2D* SceneCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;

	//sound & effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		UNiagaraSystem* NS_Shoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_Shoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_Reload;

	/** Refrence to Bullet BP*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		TSubclassOf<ABullet> BP_Bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		TSubclassOf<AGrenade> BP_Grenade;

	//Refrence to widget
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> WidgetClassPause;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> WidgetClassMap;

	/** Public Variables */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int AmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int MaxAmmo;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int GrenadeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int MaxGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int GranadeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int MaxGranade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float Wave;

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
		class UInputAction* LookInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* ShootInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* ReloadInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* ThrowInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* JumpingInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* SprintInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* PauseInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputSystem)
		class UInputAction* MapInput;

	UFUNCTION(BlueprintCallable)
		void PickUp();

	UFUNCTION(BlueprintCallable)
		void WaveSender(float Wavecount);

private:
	/** Private Functions */

	UFUNCTION(BlueprintCallable)
		void ForwardBackward(const FInputActionValue& Val);

	UFUNCTION(BlueprintCallable)
		void RightLeft(const FInputActionValue& Val);

	UFUNCTION(BlueprintCallable)
		void Look(const FInputActionValue& Val);

	UFUNCTION(BlueprintCallable)
		void Shoot(const FInputActionValue& Val);

	UFUNCTION(BlueprintCallable)
		void Reload(const FInputActionValue& Val);

	UFUNCTION(BlueprintCallable)
		void Throw(const FInputActionValue& Val);

	UFUNCTION(BlueprintCallable)
		void Sprint(const FInputActionValue& Val);
	UFUNCTION(BlueprintCallable)
		void Pause(const FInputActionValue& Val);
	
	UFUNCTION(BlueprintCallable)
		void Map(const FInputActionValue& Val);

	UFUNCTION(BlueprintCallable)
		void OnGrenadeReleased();

	UFUNCTION(BlueprintCallable)
		void OnBulletShoot();
	UFUNCTION(BlueprintCallable)
		void IsReloading();





	/** Private Vals */
private:
	float XInput;
	float YInput;
	float ReloadTime;
	bool BSprinting;
	bool BReloading;
	bool BIsPaused;
	bool BMapOpen;



	FVector ForwardVector;


};
