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

	/** StaticMesh  */
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent()); //Root is Character's capsule
	StaticMesh->AddRelativeLocation(FVector(36, 12, -30));
	StaticMesh->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));
	StaticMesh->AddRelativeRotation(FRotator(0.f,0.f,90.f));


	///** Spring Arm */
	//SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//SpringArm->SetupAttachment(GetRootComponent());
	//SpringArm->SetRelativeLocation(FVector(40, 0, 80));
	//SpringArm->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	//SpringArm->TargetArmLength = 0.f;

	//SpringArm->bEnableCameraLag = false;
	//SpringArm->CameraLagSpeed = 15.f;
	//SpringArm->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = true; //Standing still when we dont move

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	//Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(25.f, 0.5f, -10.f));
	Camera->bUsePawnControlRotation = true;


	/** Variables */
	AmmoCount = 10;
	MaxAmmo = 10;
	GranadeCount = 10;
	MaxGranade = 10;
	MovementSpeed = 500;
	Lives = 5;


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

	//get forwardvector
	ForwardVector = FVector(XInput, YInput, 0.f);
	ForwardVector = GetActorRotation().RotateVector(ForwardVector);
	ForwardVector.Normalize();

	//movement
	FVector NewLocation = GetActorLocation() + (ForwardVector * MovementSpeed * DeltaTime);
	SetActorLocation(NewLocation);

	
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
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhanceInputCom = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhanceInputCom->BindAction(ForwardBackwardInput, ETriggerEvent::Triggered, this, &AMainCharacter::ForwardBackward);
		EnhanceInputCom->BindAction(ForwardBackwardInput, ETriggerEvent::Completed, this, &AMainCharacter::ForwardBackward);

		EnhanceInputCom->BindAction(RightLeftInput, ETriggerEvent::Triggered, this, &AMainCharacter::RightLeft);
		EnhanceInputCom->BindAction(RightLeftInput, ETriggerEvent::Completed, this, &AMainCharacter::RightLeft);

		EnhanceInputCom->BindAction(LookInput, ETriggerEvent::Triggered, this, &AMainCharacter::Look);

		EnhanceInputCom->BindAction(ShootInput, ETriggerEvent::Started, this, &AMainCharacter::Shoot);
		EnhanceInputCom->BindAction(ShootInput, ETriggerEvent::Triggered, this, &AMainCharacter::Shoot);

		EnhanceInputCom->BindAction(ReloadInput, ETriggerEvent::Started, this, &AMainCharacter::Reload);

		EnhanceInputCom->BindAction(JumpingInput, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhanceInputCom->BindAction(JumpingInput, ETriggerEvent::Completed, this, &ACharacter::Jump);

	}
}
void AMainCharacter::ForwardBackward(const FInputActionValue& Val)
{
	XInput = Val.Get<float>();

	if (Controller && (XInput != 0.f))
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, XInput);
	}
}

void AMainCharacter::RightLeft(const FInputActionValue& Val)
{
	YInput = Val.Get<float>();

	if (Controller && (YInput != 0.f))
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, YInput);
	}
}

void AMainCharacter::Look(const FInputActionValue& Val)
{
	const FVector2D LookInputVal = Val.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookInputVal.X);
		AddControllerPitchInput(LookInputVal.Y);
	}
}


void AMainCharacter::Shoot(const FInputActionValue& Val)
{
	if (AmmoCount > 0) {

		AmmoCount--;
		GetWorld()->SpawnActor<AActor>(BP_Bullet,
			StaticMesh->GetComponentLocation() , GetActorRotation());

	}
}

void AMainCharacter::Reload(const FInputActionValue& Val)
{
	AmmoCount = MaxAmmo;
}

void AMainCharacter::Throw(const FInputActionValue& Val)
{
	//TODO add the Blueprint for the Grenade
/*Ammo--;
		GetWorld()->SpawnActor<AActor>(Granate_BP,
			GetActorLocation() + FVector(30.f, 0.f, 0.f), GetActorRotation());*/
}
