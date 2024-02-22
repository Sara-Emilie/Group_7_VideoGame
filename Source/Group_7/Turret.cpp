// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "MainCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "EnemyAI.h"
#include "MainCharacter.h"
#include "Bullet.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	TurretMesh->SetSimulatePhysics(false);
	TurretMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	TurretMesh->SetSimulatePhysics(true);
	RootComponent = TurretMesh;
	TurretSpeed = 2800.0f;
	bHasChecked = false;
	
	
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	TurretMesh->IgnoreActorWhenMoving(MainCharacter, true);
	TurretMesh->MoveIgnoreActors;
	bCanShoot = true;
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TurretMesh->IgnoreActorWhenMoving(MainCharacter, true);
	TurretMesh->MoveIgnoreActors;
	if (!bHasChecked) {
		float timedelay = FMath::FRandRange(2.f, 3.f);
		GetWorldTimerManager().SetTimer(TH_CheckDelay, this, &ATurret::CheckForEnemies, timedelay, false);
		bHasChecked = true;
		
		
	}
	if (bCanShoot) {
		float shootDelay = 2.1f;
		GetWorldTimerManager().SetTimer(TH_ShootDelay, this, &ATurret::Shoot, shootDelay, false);
		bCanShoot = false;
	}

}

void ATurret::OnReleasedTurret(FVector ForWardVector)
{
	ForWardVector *= TurretSpeed;

	TurretMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	TurretMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TurretMesh->SetSimulatePhysics(true);
	TurretMesh->AddImpulse(ForWardVector);
	
	
}

void ATurret::Shoot()
{
	FVector SpawnLocation = TurretMesh->GetComponentLocation() + FVector(0.0f, 0.0f, 55.0f);
	FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::GetForwardVector(GetActorRotation()).GetSafeNormal());


	Bullet = GetWorld()->SpawnActor<ABullet>(BP_Bullet, SpawnLocation, GetActorRotation());
	if (Bullet)
	{
		Bullet->OnBulletShoot(UKismetMathLibrary::GetForwardVector(GetActorRotation()).GetSafeNormal());
	}
	bCanShoot = true;
}

void ATurret::CheckForEnemies()
{
	TArray<AActor*> AliveEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAI::StaticClass(), AliveEnemies);

	// Assuming turret is targeting the closest enemy
	AEnemyAI* ClosestEnemy = nullptr;
	float ClosestDistance = 5500.0f;

	for (AActor* EnemyActor : AliveEnemies)
	{
		AEnemyAI* Enemy = Cast<AEnemyAI>(EnemyActor);
		if (Enemy)
		{
			// Calculate the distance between the turret and the enemy
			float DistanceToEnemy = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());

			// Check if this is the closest enemy so far
			if (DistanceToEnemy < ClosestDistance)
			{
				ClosestDistance = DistanceToEnemy;
				ClosestEnemy = Enemy;
			}
		}
	}

	if (ClosestEnemy)
	{
		// Aim at the closest enemy
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ClosestEnemy->GetActorLocation());
		SetActorRotation(LookAtRotation);

		// Do something with the targeted enemy
		
		FVector TargetLocation = ClosestEnemy->GetActorLocation();
		

	}
	
	bHasChecked = false;
}

