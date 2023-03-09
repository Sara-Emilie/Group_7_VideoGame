// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

			/** StaticMesh (Root Component) */
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetRelativeLocation(FVector(0, 0, 0));
	StaticMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

			/** Spring Arm */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.f;
	SpringArm->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false; //Standing still when we dont move

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

			/** Variables */
	AmmoCount = 10;
	MaxAmmo = 10;
	GranadeCount = 10;
	MaxGranade = 10;
	MovementSpeed = 1000;
	Lives = 5;


	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	AutoPossessPlayer = EAutoReceiveInput::Player0; //Possesses this as the player

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController) {
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (subsystem) {
			subsystem->AddMappingContext(IMC, 0);

		}
	}

}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ForwardVector = FVector(XInput, YInput, 0.f);
	ForwardVector.Normalize();

	FVector NewLocation = GetActorLocation() + (ForwardVector * MovementSpeed * DeltaTime);
	SetActorLocation(NewLocation);

	// to stop crashing
	AddControllerPitchInput(Pitch);
	AddControllerYawInput(Yaw);
	if ((Controller != nullptr) && (XInput != 0.0f))
	{
		FRotator Rotation = Controller->GetControlRotation();
		Rotation.Pitch = 0.f;
		Rotation.Roll = 0.f;

		// get the local forward vector normalized
		FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		SetActorLocation(GetActorLocation() + (Direction * XInput * MovementSpeed * DeltaTime));

		SetActorRotation(Rotation);
	}
	if ((Controller != nullptr) && (YInput != 0.0f))
	{
		FRotator Rotation = Controller->GetControlRotation();
		Rotation.Pitch = 0.f;
		Rotation.Roll = 0.f;

		// get the local forward vector normalized
		FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
		SetActorLocation(GetActorLocation() + (Direction * YInput * MovementSpeed * DeltaTime));

		SetActorRotation(Rotation);
	}

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhanceInputCom = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhanceInputCom->BindAction(ForwardBackwardInput, ETriggerEvent::Triggered, this, &APlayerPawn::ForwardBackward);
		EnhanceInputCom->BindAction(ForwardBackwardInput, ETriggerEvent::Completed, this, &APlayerPawn::ForwardBackward);

		EnhanceInputCom->BindAction(RightLeftInput, ETriggerEvent::Triggered, this, &APlayerPawn::RightLeft);
		EnhanceInputCom->BindAction(RightLeftInput, ETriggerEvent::Completed, this, &APlayerPawn::RightLeft);

		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Completed, this, &APlayerPawn::MouseX);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Triggered, this, &APlayerPawn::MouseX);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Started, this, &APlayerPawn::MouseX);

		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Completed, this, &APlayerPawn::MouseY);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Triggered, this, &APlayerPawn::MouseY);
		
		EnhanceInputCom->BindAction(ShootInput, ETriggerEvent::Started, this, &APlayerPawn::Shoot);
		EnhanceInputCom->BindAction(ShootInput, ETriggerEvent::Triggered, this, &APlayerPawn::Shoot);
		
		EnhanceInputCom->BindAction(ReloadInput, ETriggerEvent::Started, this, &APlayerPawn::Reload);
		

	}
}

void APlayerPawn::ForwardBackward(const FInputActionValue& input)
{
	XInput = input.Get<float>();
}

void APlayerPawn::RightLeft(const FInputActionValue& input)
{
	YInput = input.Get<float>();

}

void APlayerPawn::Shoot(const FInputActionValue& input)
{
	if (AmmoCount > 0) {

		//TODO add the Blueprint for the Bullet

		/*Ammo--;
		GetWorld()->SpawnActor<AActor>(Bullet_BP,
			GetActorLocation() + FVector(30.f, 0.f, 0.f), GetActorRotation());*/


	}
}

void APlayerPawn::Reload(const FInputActionValue& input)
{
	AmmoCount = MaxAmmo;
}

void APlayerPawn::Throw(const FInputActionValue& input)
{

	//TODO add the Blueprint for the Granate
	/*Ammo--;
			GetWorld()->SpawnActor<AActor>(Granate_BP,
				GetActorLocation() + FVector(30.f, 0.f, 0.f), GetActorRotation());*/

}

void APlayerPawn::MouseX(const FInputActionValue& input)
{
	Yaw = input.Get<float>();
	AddControllerYawInput(Yaw);
}

void APlayerPawn::MouseY(const FInputActionValue& input)
{
	Pitch = input.Get<float>();
	AddControllerPitchInput(Pitch);
}
