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


	/** Variables */
	AmmoCount = 10;
	MaxAmmo = 10;
	GrenadeCount = 3;
	MaxGrenade = 3;
	MovementSpeed = 25;
	Lives = 5;
	BSprinting = false;
	BReloading = false;
	BIsPaused = false;
	BMapOpen = true;
	ReloadTime = 1.f;
	TimePassed = 0.f;
	ZOfSet = 0.f;
	ZSprintMultiplier = 0.05f;

	
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

	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////get forwardvector
	//ForwardVector = FVector(XInput, YInput, 0.f);
	//ForwardVector = GetActorRotation().RotateVector(ForwardVector);
	//ForwardVector.Normalize();

	////movement
	//FVector NewLocation = GetActorLocation() + (ForwardVector * MovementSpeed * DeltaTime);
	//SetActorLocation(NewLocation);


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
	ZOfSet = ZSprintMultiplier * FMath::Sin(TimePassed * 5);
	

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

		EnhanceInputCom->BindAction(JumpingInput, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhanceInputCom->BindAction(JumpingInput, ETriggerEvent::Completed, this, &ACharacter::Jump);

		EnhanceInputCom->BindAction(SprintInput, ETriggerEvent::Started, this, &AMainCharacter::Sprint);
		EnhanceInputCom->BindAction(SprintInput, ETriggerEvent::Completed, this, &AMainCharacter::Sprint);

		EnhanceInputCom->BindAction(PauseInput, ETriggerEvent::Triggered, this, &AMainCharacter::Pause);	

		EnhanceInputCom->BindAction(MapInput, ETriggerEvent::Started, this, &AMainCharacter::Map);

		

	}
}
void AMainCharacter::ForwardBackward(const FInputActionValue& Val)
{
	XInput = Val.Get<float>();
	StaticMesh->AddLocalOffset(FVector(0, 0, ZOfSet));
	MuzzleSpawnMesh->AddLocalOffset(FVector(0, 0, ZOfSet));
	if (Controller && (XInput != 0.f))
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, XInput);
	}
}

void AMainCharacter::RightLeft(const FInputActionValue& Val)
{
	YInput = Val.Get<float>();
	StaticMesh->AddLocalOffset(FVector(0, 0, ZOfSet));
	MuzzleSpawnMesh->AddLocalOffset(FVector(0, 0, ZOfSet));

	if (Controller && (YInput != 0.f))
	{
		FVector Forward = GetActorRightVector();
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
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Reload called"));

		WBP_Reload = CreateWidget<UUserWidget>(GetGameInstance(), WidgetReload);
		WBP_Reload->AddToViewport();

		GetWorldTimerManager().SetTimer(TReloadHandle, this, &AMainCharacter::IsReloading, ReloadTime, false);
		//all_timer_handles.Add(TReloadHandle);
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
			if(Grenade)
			{
				//For animations 
				/*AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);*/
			}

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
		ZSprintMultiplier = 0.05f;
	}
	else
	{
		BSprinting = true;
		MovementSpeed = 250;
		ZSprintMultiplier = 0.1f;
	}
	
}

void AMainCharacter::Pause(const FInputActionValue& Val)
{
	
	//if(BIsPaused)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("GamePaused"));
	//BIsPaused = false;
		GetWorld()->GetFirstPlayerController()->Pause();
		WBP_Pause_Screen = CreateWidget<UUserWidget>(GetGameInstance(), WidgetClassPause);
		WBP_Pause_Screen->AddToViewport();

		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("GameResumed"));
	//	BIsPaused = true;
	//	WBP_Pause_Screen->RemoveFromParent();
	//	GetWorld()->GetFirstPlayerController()->Pause();

	//	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	//	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	//	
	//}
}

void AMainCharacter::Map(const FInputActionValue& Val)
{
	
	if (BMapOpen)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("MapOpen"));
		BMapOpen = false;
		WBP_BigMap = CreateWidget<UUserWidget>(GetGameInstance(), WidgetClassMap);
		WBP_BigMap->AddToViewport();
	}

	else if(BMapOpen == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("MapClose"));
		
		WBP_BigMap->RemoveFromParent();
		BMapOpen = true;
	}
}


void AMainCharacter::OnGrenadeReleased()
{
	if (Grenade)
	{
		Grenade->OnReleased(UKismetMathLibrary::GetForwardVector(GetControlRotation()));
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
	
	//for (int i = 0; i < all_timer_handles.Num(); i++)
	//{
	//	GetWorldTimerManager().ClearTimer(all_timer_handles[i]);
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("all_timer_handles[i]"));
	//}
	//GetWorldTimerManager().ClearAllTimersForObject(this);
}

void AMainCharacter::PickUp()
{
	if(GrenadeCount < MaxGrenade)
	{
		GrenadeCount++;
	}
	//if (GrenadeCount >= MaxGranade) // failsafe 
	//{
	//	GrenadeCount = MaxGrenade;
	//}
}


