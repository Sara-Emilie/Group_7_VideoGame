// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPowerUp.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

// Sets default values
AAmmoPowerUp::AAmmoPowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);


	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	PickUpMesh->SetupAttachment(Collider);

	PickUpMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

}

// Called when the game starts or when spawned
void AAmmoPowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPowerUp::OnOverlap);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called every frame
void AAmmoPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoPowerUp::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMainCharacter>())
	{
		Cast<AMainCharacter>(OtherActor)->AmmoMagBoost();
		DestroyPickUp();
	}
}

void AAmmoPowerUp::DestroyPickUp()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();
}

