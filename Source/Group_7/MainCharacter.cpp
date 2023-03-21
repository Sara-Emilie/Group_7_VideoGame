// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "Bullet.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	/** StaticMesh (Root Component) */
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetRelativeLocation(FVector(0, 0, 0));
	StaticMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	/** Capsule */
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	//TODO set size after character size here
	SetRootComponent(GetRootComponent());

	/** Spring Arm */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeLocation(FVector(40, 0, 80));
	SpringArm->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 0.f;

	SpringArm->bEnableCameraLag = false;
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
	IsJumping = false;


	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	AutoPossessPlayer = EAutoReceiveInput::Player0; //Possesses this as the player

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
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
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ForwardVector = FVector(XInput, YInput, 0.f);

	ForwardVector = GetActorRotation().RotateVector(ForwardVector);
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
	if(IsJumping)
	{
		Jump();
	}

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhanceInputCom = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhanceInputCom->BindAction(ForwardBackwardInput, ETriggerEvent::Triggered, this, &AMainCharacter::ForwardBackward);
		EnhanceInputCom->BindAction(ForwardBackwardInput, ETriggerEvent::Completed, this, &AMainCharacter::ForwardBackward);

		EnhanceInputCom->BindAction(RightLeftInput, ETriggerEvent::Triggered, this, &AMainCharacter::RightLeft);
		EnhanceInputCom->BindAction(RightLeftInput, ETriggerEvent::Completed, this, &AMainCharacter::RightLeft);

		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Completed, this, &AMainCharacter::MouseX);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Triggered, this, &AMainCharacter::MouseX);
		EnhanceInputCom->BindAction(MouseXInput, ETriggerEvent::Started, this, &AMainCharacter::MouseX);

		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Completed, this, &AMainCharacter::MouseY);
		EnhanceInputCom->BindAction(MouseYInput, ETriggerEvent::Triggered, this, &AMainCharacter::MouseY);

		EnhanceInputCom->BindAction(ShootInput, ETriggerEvent::Started, this, &AMainCharacter::Shoot);
		EnhanceInputCom->BindAction(ShootInput, ETriggerEvent::Triggered, this, &AMainCharacter::Shoot);

		EnhanceInputCom->BindAction(ReloadInput, ETriggerEvent::Started, this, &AMainCharacter::Reload);

		EnhanceInputCom->BindAction(JumpingInput, ETriggerEvent::Started, this, &AMainCharacter::Jumping);
		EnhanceInputCom->BindAction(JumpingInput, ETriggerEvent::Completed, this, &AMainCharacter::Jumping);

	}
}
void AMainCharacter::ForwardBackward(const FInputActionValue& input)
{
	XInput = input.Get<float>();
}

void AMainCharacter::RightLeft(const FInputActionValue& input)
{
	YInput = input.Get<float>();
}

void AMainCharacter::Shoot(const FInputActionValue& input)
{
	if (AmmoCount > 0) {

		//TODO add the Blueprint for the Bullet

		AmmoCount--;
		GetWorld()->SpawnActor<AActor>(BP_Bullet,
			GetActorLocation() + FVector(30.f, 0.f, 0.f), GetActorRotation());


	}
}

void AMainCharacter::Reload(const FInputActionValue& input)
{
	AmmoCount = MaxAmmo;
}

void AMainCharacter::Throw(const FInputActionValue& input)
{
	//TODO add the Blueprint for the Grenade
/*Ammo--;
		GetWorld()->SpawnActor<AActor>(Granate_BP,
			GetActorLocation() + FVector(30.f, 0.f, 0.f), GetActorRotation());*/
}

void AMainCharacter::MouseX(const FInputActionValue& input)
{
	Yaw = input.Get<float>();
	/*auto f = GetActorRotation();
	f.Yaw += Yaw;
	SetActorRotation(f);*/
	AddControllerYawInput(Yaw);
	AddActorLocalRotation(FRotator(0, Yaw, 0));
}

void AMainCharacter::MouseY(const FInputActionValue& input)
{
	Pitch = input.Get<float>();
	AddControllerPitchInput(Pitch);
	AddActorLocalRotation(FRotator(0, Pitch, 0));
}

void AMainCharacter::Jumping(const FInputActionValue& input)
{
	if(IsJumping)
	{
		IsJumping = false;
	}
	else
	{
		IsJumping = true; 
	}
}
