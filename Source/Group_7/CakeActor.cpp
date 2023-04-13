// Fill out your copyright notice in the Description page of Project Settings.


#include "CakeActor.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"


// Sets default values
class AEnemy;
ACakeActor::ACakeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CakeHealth = 10;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACakeActor::OnOverlap);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);



}

// Called when the game starts or when spawned
void ACakeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACakeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACakeActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// collision between the cake and enemy

	if (OtherActor->IsA<AEnemy>()) {
		Cast<AEnemy>(OtherActor)->GotTheCake();
		DamageCake();

	}
}

void ACakeActor::DamageCake()
{
	CakeHealth--;
	

	Cast<AMainCharacter>(MainCharacter)->CakeHealthSender(CakeHealth);
	if (CakeHealth <= 0) {
		bCakeDestoryed = true;
		// lose the game
		// add a losing level, if time add explosion
	}
}

