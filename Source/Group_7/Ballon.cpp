// Fill out your copyright notice in the Description page of Project Settings.


#include "Ballon.h"

// Sets default values
ABallon::ABallon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BalloonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BalloonMesh"));
	SetRootComponent(BalloonMesh);



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

