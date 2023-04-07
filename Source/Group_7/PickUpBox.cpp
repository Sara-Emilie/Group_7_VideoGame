// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBox.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"


// Sets default values
APickUpBox::APickUpBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);


	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	PickUpMesh->SetupAttachment(Collider);

	PickUpMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	DespawnTimer = 0.f;
	lifeSpan = 10.f;
}

// Called when the game starts or when spawned
void APickUpBox::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APickUpBox::OnOverlap);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

// Called every frame
void APickUpBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	DespawnTimer += DeltaTime;
	if (DespawnTimer >= lifeSpan) {
		DestroyPickUp();
	}
}

void APickUpBox::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMainCharacter>()) {

		UE_LOG(LogTemp, Warning, TEXT("U got ammo"));
		Cast<AMainCharacter>(OtherActor)->PickUp();
		DestroyPickUp();
	}
}

void APickUpBox::DestroyPickUp()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	this->Destroy();
}

