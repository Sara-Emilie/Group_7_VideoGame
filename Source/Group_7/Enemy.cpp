// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Bullet.h"
#include "InputTriggers.h"

class ABullet; 

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent()); //Root is Character's capsule
	StaticMesh->SetRelativeLocation(FVector(0, 0, 0));
	StaticMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	bUseControllerRotationYaw = false;

	MovementSpeed = 1000;
	EnemyHealth = 2;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ABullet>()) {
		Cast<ABullet>(OtherActor)->DestroyBullet();
		UE_LOG(LogTemp, Warning, TEXT("Damage Taken"));
		TakeDamage();
		
	/*	EnemyHealth--;*/

		//if (EnemyHealth <= 0)
		//{
		//	DestoryTarget();
		//}
	}
}

void AEnemy::DestoryTarget()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	//AEnemySpawner* Spawner;

	this->Destroy();
}

void AEnemy::TakeDamage()
{
	EnemyHealth--;
	if (EnemyHealth <= 0)
	{
		DestoryTarget();
	}
}

void AEnemy::GotTheCake()
{
	//touched the cake
	
	DestoryTarget();
}



