// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGrenade::AGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Grenade
	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	GrenadeMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	RootComponent = GrenadeMesh;

	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	FuseTime = 5.f;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	/*GetWorld()->GetTimerManager().SetTimer(LifeSpan, this, &AGrenade::DestroyGrenade, 1.f, false);*/
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::OnReleased(FVector ForWardVector)
{
	ForWardVector *= 2500;

	GrenadeMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GrenadeMesh->SetSimulatePhysics(true);
	GrenadeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	GrenadeMesh->AddImpulse(ForWardVector);

	FTimerHandle TExsplodeHandle;
	GetWorldTimerManager().SetTimer(TExsplodeHandle, this, &AGrenade::Exsplode, FuseTime, false);
}



void AGrenade::Exsplode()
{
	FVector ExsplotionLocation = GrenadeMesh->GetComponentLocation();

	if (NS_Explosion)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Explosion, ExsplotionLocation);
	}

	if (SB_Explosion)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Explosion, ExsplotionLocation, FRotator::ZeroRotator);


	}

	DamageSphere->SetWorldLocation(ExsplotionLocation);

	TArray<AActor*> OverlappingActors;
	DamageSphere->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (AMainCharacter* Character = Cast<AMainCharacter>(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Exploded an actor: %s"), *Character->GetName());
		}
	}
	Destroy();
}
