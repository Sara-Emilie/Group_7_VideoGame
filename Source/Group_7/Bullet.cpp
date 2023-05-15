// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "EnemyAI.h"
#include "Ballon.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(Collider);
	StaticMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	BulletSpeed = 2000.0f;
	DespawnTimer = 0.f;
	lifeSpan = 2.f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collider->SetCollisionObjectType(ECC_Pawn);
	Collider->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Collider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DespawnTimer += DeltaTime;
	if (DespawnTimer >= lifeSpan) {
		DestroyBullet();
	}
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Collided on: %s"), *UKismetSystemLibrary::GetDisplayName(OtherActor)));*/

	if (OtherActor->IsA<AEnemyAI>()) {

		Cast<AEnemyAI>(OtherActor)->EnemyTakeDamage();
		DestroyBullet();
	}
	if (OtherActor->IsA<ABallon>()) {

		Cast<ABallon>(OtherActor)->BalloonPop();
		DestroyBullet();
	}
}

void ABullet::DestroyBullet()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();
}

void ABullet::OnBulletShoot(FVector ForWardVector)
{
	ForWardVector *= 100 * BulletSpeed;

	Collider->SetSimulatePhysics(true);
	Collider->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Collider->AddImpulse(ForWardVector);

}

