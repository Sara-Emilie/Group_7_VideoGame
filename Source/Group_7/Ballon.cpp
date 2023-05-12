// Fill out your copyright notice in the Description page of Project Settings.


#include "Ballon.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
ABallon::ABallon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetRelativeLocation(FVector(0, 0, 20.f));
	Collider->SetSphereRadius(90);
	SetRootComponent(Collider);
	
	BalloonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BalloonMesh"));
	BalloonMesh->SetupAttachment(Collider);

	TimePassed = 0;
	ZOfSet = 0;
	XOfSet = 0;
	YOfSet = 0; 

}

// Called when the game starts or when spawned
void ABallon::BeginPlay()
{
	Super::BeginPlay();
	scale = FMath::RandRange(1, 10);
	scale = scale / 10;

}

// Called every frame
void ABallon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimePassed += DeltaTime;
	ZOfSet = scale * FMath::Sin(TimePassed * (scale + 2));
	XOfSet = scale * FMath::Cos(TimePassed * (scale + 2));
	YOfSet = scale * FMath::Cos(TimePassed * (scale + 2));
	
	
	AddActorWorldOffset(FVector(XOfSet, YOfSet, ZOfSet));


}

void ABallon::BalloonPop()
{
	if (NS_BalloonPop)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_BalloonPop, Collider->GetComponentLocation());
	}

	if (SB_BalloonPop)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_BalloonPop, Collider->GetComponentLocation(), FRotator::ZeroRotator);
	}

	this->Destroy();
}

