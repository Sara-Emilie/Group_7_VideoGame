// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Enemy.h"
#include "Grenade.h"

#include "EnemySpawner.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	//Collider->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap); //Will never work
	

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(Collider);
	StaticMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	BulletSpeed = 2000.0f;
	DespawnTimer = 0.f;
	lifeSpan = 1.f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetCollisionObjectType(ECC_Pawn);
	Collider->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Collider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation();
	NewLocation += GetActorForwardVector() * BulletSpeed * DeltaTime;
	NewLocation.Z += this->GetActorRotation().Pitch * BulletSpeed * DeltaTime;

	SetActorLocation(NewLocation);

	//SetActorLocation(NewLocation);
	DespawnTimer += DeltaTime;
	if (DespawnTimer >= lifeSpan) {
		DestroyBullet();
	}
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Collided on: %s"), *UKismetSystemLibrary::GetDisplayName(OtherActor)));

	if (OtherActor->IsA<AEnemy>()) {

		UE_LOG(LogTemp, Warning, TEXT("U got hit mohahahah"));
		Cast<AEnemy>(OtherActor)->TakeDamage();
		DestroyBullet();
		UE_LOG(LogTemp, Warning, TEXT("U got hit mohahahah"));
	}
	//if (OtherActor->IsA<AGrenade>())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Grenade hit"));
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("HELLO"));


	//	Cast<AGrenade>(OtherActor)->Exsplode();
	//	
	//	DestroyBullet();
	//}
}

void ABullet::DestroyBullet()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();
}

//void ABullet::OnBulletShoot(FVector ForWardVector)
//{
	/*ForWardVector *= 100 * BulletSpeed;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	StaticMesh->AddImpulse(ForWardVector);*/

//}

