// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetRelativeLocation(FVector(0, 0, 0));
	StaticMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.f;
	SpringArm->bUsePawnControlRotation = true;

	// Stand still when we dont move
	bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	AmmoCount = 10;
	MaxAmmo = 10;
	GranadeCount = 10;
	MaxGranade = 10;
	MovementSpeed = 1000;
	Lives = 5;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//void APlayerPawn::ForwardBackward(const FInputActionValue& input)
//{
//}
//
//void APlayerPawn::RightLeft(const FInputActionValue& input)
//{
//}
//
//void APlayerPawn::Shoot(const FInputActionValue& input)
//{
//}
//
//void APlayerPawn::Reload(const FInputActionValue& input)
//{
//}
//
//void APlayerPawn::Throw(const FInputActionValue& input)
//{
//}
//
//void APlayerPawn::MouseX(const FInputActionValue& input)
//{
//}
//
//void APlayerPawn::MouseY(const FInputActionValue& input)
//{
//}
