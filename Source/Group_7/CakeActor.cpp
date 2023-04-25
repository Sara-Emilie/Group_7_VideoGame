// Fill out your copyright notice in the Description page of Project Settings.


#include "CakeActor.h"
#include "EnemyAI.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"


// Sets default values
class AEnemyAI;
ACakeActor::ACakeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACakeActor::OnOverlap);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);



	CakeHealth = 10;
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
	// collision between the cake and enemy TODO dont work for EnemyAI

	if (OtherActor->IsA<AEnemyAI>()) {
		Cast<AEnemyAI>(OtherActor)->GotTheCake();
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Enemy touched the cake"));
		DamageCake();

	}
}

void ACakeActor::DamageCake()
{
	CakeHealth--;


	//Cast<AMainCharacter>(MainCharacter)->CakeHealthSender(CakeHealth);
	if (CakeHealth <= 0)
	{
		GetWorld()->GetFirstPlayerController()->Pause();
		UUserWidget* WGP_GameOver = CreateWidget<UUserWidget>(GetGameInstance(), WidgetClassGameOver);
		WGP_GameOver->AddToViewport();

		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
}
