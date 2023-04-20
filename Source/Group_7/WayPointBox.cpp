// Fill out your copyright notice in the Description page of Project Settings.


#include "WayPointBox.h"
#include "Components/BoxComponent.h"

// Sets default values
AWayPointBox::AWayPointBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
}

// Called when the game starts or when spawned
void AWayPointBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWayPointBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AWayPointBox::GetWayPointOrder()
{
	return WayPointOrder;
}

