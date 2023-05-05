// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAI.h"

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

	FuseTime = 1.f;
	MaxGrenadeSpeed = 50;
	GrenadeSpeed = 2500;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::OnReleased(FVector ForWardVector)
{
	if (ForWardVector.Size() >= MaxGrenadeSpeed)
		GrenadeSpeed = 1000;

	ForWardVector *= GrenadeSpeed;

	GrenadeMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GrenadeMesh->SetSimulatePhysics(true);
	GrenadeMesh->AddImpulse(ForWardVector);

	FTimerHandle TExsplodeHandle;
	GetWorldTimerManager().SetTimer(TExsplodeHandle, this, &AGrenade::Explode, FuseTime, false);
}



void AGrenade::Explode()
{
	FVector ExsplotionLocation = GrenadeMesh->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Grenade boom"));
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("HELLO B"));

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
	DamageSphere->GetOverlappingActors(OverlappingActors, TSubclassOf<AEnemyAI>());

	for (AActor* Actor : OverlappingActors)
	{
		if (AEnemyAI* Enemy = Cast<AEnemyAI>(Actor))
		{
			Cast<AEnemyAI>(Enemy)->EnemyTakeDamage();
			Enemy->EnemyTakeDamage();

		}
	}
	Destroy();
}

