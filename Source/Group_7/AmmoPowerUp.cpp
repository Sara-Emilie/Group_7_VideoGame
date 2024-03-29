// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPowerUp.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
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

	Scale = 1.f;
}

// Called every frame
void AAmmoPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimePassed += DeltaTime;
	ZOffset = Scale * FMath::Sin(TimePassed * (Scale + 2));

	AddActorWorldOffset(FVector(0.f, 0.f, ZOffset));

}

void AAmmoPowerUp::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMainCharacter>())
	{
		Cast<AMainCharacter>(OtherActor)->AmmoMagBoost();
		if (SB_Pickup)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Pickup, GetActorLocation(), GetActorRotation()); //, FRotator::ZeroRotator);
		}
		DestroyPickUp();
	}
}

void AAmmoPowerUp::DestroyPickUp()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();
}

