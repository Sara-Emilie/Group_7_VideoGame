// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "InputTriggers.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Turret.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;




	bUseControllerRotationYaw = true; //Standing still when we dont move

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(GetRootComponent()); //Root is Character's capsule
	Camera->SetRelativeLocation(FVector(25.f, 0.5f, -10.f));
	Camera->bUsePawnControlRotation = true;

	/** StaticMesh  */
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	StaticMesh->SetupAttachment(Camera);
	StaticMesh->AddRelativeLocation(FVector(36, 12, -30));
	StaticMesh->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));
	StaticMesh->AddRelativeRotation(FRotator(0.f, 0.f, 90.f));

	//so muzzle flash spawns on tip of gun
	MuzzleSpawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleSpawn"));
	MuzzleSpawnMesh->SetupAttachment(Camera);
	MuzzleSpawnMesh->AddRelativeLocation(FVector(54, 18, -10));
	MuzzleSpawnMesh->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));


	/** SpringArm */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;
	SpringArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));


	/** Variables & Booleans */
	AmmoCount = 10;
	MaxAmmo = 10;
	GrenadeCount = 3;
	MaxGrenade = 3;
	MovementSpeed = 25;
	Lives = 5;
	BSprinting = false;
	BReloading = false;
	BIsPaused = true;
	BMapOpen = true;
	ReloadTime = 1.f;
	TimePassed = 0.f;
	ZOfSet = 0.f;
	ZSprintMultiplier = 0.05f;
	MaxTurretCount = 3;
	TurretCount = 3;
	

	
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	

	AutoPossessPlayer = EAutoReceiveInput::Player0; //Possesses this as the player

	
}


// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController) {
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (subsystem) {
			subsystem->AddMappingContext(IMC, 0);

		}
	}
	if (SB_MainTheme)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_MainTheme, StaticMesh->GetComponentLocation(), GetActorRotation());
	}


	ZSprintMultiplier = 5.f;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZOfSet = 0.025 * FMath::Sin(TimePassed * ZSprintMultiplier);
	StaticMesh->AddLocalOffset(FVector(0, 0, ZOfSet));
	MuzzleSpawnMesh->AddLocalOffset(FVector(0, 0, ZOfSet));


	//Just to be safe:
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

	TimePassed += DeltaTime;
	if (AmmoCount <= 0)
	{

		if (BReloading == false)
		{
			BReloading = true;

			WBP_Reload = CreateWidget<UUserWidget>(GetGameInstance(), WidgetReload);
			WBP_Reload->AddToViewport();

			GetWorldTimerManager().SetTimer(TReloadHandle, this, &AMainCharacter::IsReloading, ReloadTime, false);

			if (SB_Reload) {
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Reload, StaticMesh->GetComponentLocation(), StaticMesh->GetComponentRotation());
			}

		}
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

		EnhanceInputCom->BindAction(ReloadInput, ETriggerEvent::Started, this, &AMainCharacter::Reload);

		EnhanceInputCom->BindAction(ThrowInput, ETriggerEvent::Started, this, &AMainCharacter::Throw);
		EnhanceInputCom->BindAction(TurretInput, ETriggerEvent::Started, this, &AMainCharacter::ThrowTurret);

		EnhanceInputCom->BindAction(JumpingInput, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhanceInputCom->BindAction(JumpingInput, ETriggerEvent::Completed, this, &ACharacter::Jump);

		EnhanceInputCom->BindAction(SprintInput, ETriggerEvent::Started, this, &AMainCharacter::Sprint);
		EnhanceInputCom->BindAction(SprintInput, ETriggerEvent::Completed, this, &AMainCharacter::Sprint);

		EnhanceInputCom->BindAction(PauseInput, ETriggerEvent::Started, this, &AMainCharacter::Pause);	

		EnhanceInputCom->BindAction(MapInput, ETriggerEvent::Started, this, &AMainCharacter::Map);

		

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
		FVector Right = GetActorRightVector();
		AddMovementInput(Right, YInput);
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
	if (BReloading == true)
		return;
	if (AmmoCount > 0) {

		AmmoCount--;
		if(BP_Bullet)
		{
			Bullet = GetWorld()->SpawnActor<ABullet>(BP_Bullet, StaticMesh->GetComponentLocation(), GetActorRotation());

			if (NS_Shoot)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(NS_Shoot, MuzzleSpawnMesh, FName("MuzzleSocket"), GetOwner()->GetTargetLocation(), GetOwner()->GetActorRotation(), EAttachLocation::SnapToTarget, false);
			} 

			if (SB_Shoot)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Shoot, StaticMesh->GetComponentLocation(), GetActorRotation());
			}

			OnBulletShoot();
		}
	}
}

void AMainCharacter::Reload(const FInputActionValue& Val)
{
	if(BReloading == false)
	{
		BReloading = true;

		WBP_Reload = CreateWidget<UUserWidget>(GetGameInstance(), WidgetReload);
		WBP_Reload->AddToViewport();

		GetWorldTimerManager().SetTimer(TReloadHandle, this, &AMainCharacter::IsReloading, ReloadTime, false);

		if (SB_Reload) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Reload, StaticMesh->GetComponentLocation(), StaticMesh->GetComponentRotation());
		}

	}
	
}

void AMainCharacter::Throw(const FInputActionValue& Val)
{
	if(GrenadeCount > 0)
	{
		GrenadeCount--;
		if (BP_Grenade)
		{
			Grenade = GetWorld()->SpawnActor<AGrenade>(BP_Grenade, StaticMesh->GetComponentLocation(), GetActorRotation());

			OnGrenadeReleased();
		}
	}

}

void AMainCharacter::Sprint(const FInputActionValue& Val)
{
	if (BSprinting)
	{
		BSprinting = false;
		MovementSpeed = 25;
		ZSprintMultiplier = 2.1f;
	}
	else
	{
		BSprinting = true;
		MovementSpeed = 250;
		ZSprintMultiplier = 6.f;
	}
	
}

void AMainCharacter::Pause(const FInputActionValue& Val)
{
	
	if(BIsPaused)
	{
		BIsPaused = false;
		GetWorld()->GetFirstPlayerController()->Pause();
		WBP_Pause_Screen = CreateWidget<UUserWidget>(GetGameInstance(), WidgetClassPause);
		WBP_Pause_Screen->AddToViewport();

		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	}

	else
	{

		BIsPaused = true;
		WBP_Pause_Screen->RemoveFromParent();
		GetWorld()->GetFirstPlayerController()->Pause();
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		
	}
}

void AMainCharacter::Map(const FInputActionValue& Val)
{
	
	if (BMapOpen)
	{
		BMapOpen = false;
		WBP_BigMap = CreateWidget<UUserWidget>(GetGameInstance(), WidgetClassMap);
		WBP_BigMap->AddToViewport();
	}

	else if(BMapOpen == false)
	{
		WBP_BigMap->RemoveFromParent();
		BMapOpen = true;
	}
}




void AMainCharacter::ThrowTurret(const FInputActionValue& Val)
{
	if (TurretCount > 0) {
		TurretCount--;
		Turret = GetWorld()->SpawnActor<ATurret>(BP_Turret, StaticMesh->GetComponentLocation() + FVector(0.0f, -100.0f, 0.0f), GetActorRotation());
		Turret->OnReleasedTurret(UKismetMathLibrary::GetForwardVector(GetControlRotation()));
		
	}
	
	
	
}

void AMainCharacter::OnGrenadeReleased()
{
	//gives the grenade a refrence to the character speed to add to it's speed
	if(!BSprinting)
	{
		if (Grenade)
		{
			Grenade->OnReleased(UKismetMathLibrary::GetForwardVector(GetControlRotation()));
		}
	}
	//dobbles the speed the grenade has when running
	if(BSprinting)
	{
		if (Grenade)
		{
			Grenade->OnReleased(UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 2);
		}
	}
}

void AMainCharacter::OnBulletShoot()
{
	if (Bullet)
	{
		Bullet->OnBulletShoot(UKismetMathLibrary::GetForwardVector(GetControlRotation()));
	}
}

void AMainCharacter::IsReloading()
{
	
	AmmoCount = MaxAmmo;
	BReloading = false;
	
	WBP_Reload->RemoveFromParent();
}

void AMainCharacter::PickUp()
{
	if(GrenadeCount < MaxGrenade)
	{
		GrenadeCount++;
	}
	if (TurretCount < MaxTurretCount) {
		TurretCount++;
	}
}

void AMainCharacter::AmmoMagBoost()
{
	MaxAmmo = MaxAmmo + 2;
}


