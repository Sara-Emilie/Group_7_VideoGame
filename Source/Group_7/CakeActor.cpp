// Fill out your copyright notice in the Description page of Project Settings.


#include "CakeActor.h"
#include "EnemyAI.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "Sound/SoundBase.h"
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

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	StaticMesh->AddRelativeLocation(FVector(0, 0, 0));
	StaticMesh->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	StaticMesh->AddRelativeRotation(FRotator(0.f, 0.f, .0f));

	StaticMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh1"));
	StaticMesh1->SetupAttachment(GetRootComponent());
	StaticMesh1->AddRelativeLocation(FVector(0, 0, 0));
	StaticMesh1->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	StaticMesh1->AddRelativeRotation(FRotator(0.f, 0.f, .0f));

	StaticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh2"));
	StaticMesh2->SetupAttachment(GetRootComponent());
	StaticMesh2->AddRelativeLocation(FVector(0, 0, 0));
	StaticMesh2->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	StaticMesh2->AddRelativeRotation(FRotator(0.f, 0.f, .0f));

	StaticMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh3"));
	StaticMesh3->SetupAttachment(GetRootComponent());
	StaticMesh3->AddRelativeLocation(FVector(0, 0, 0));
	StaticMesh3->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	StaticMesh3->AddRelativeRotation(FRotator(0.f, 0.f, .0f));



	CakeHealth = 10;
}

// Called when the game starts or when spawned
void ACakeActor::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh1->SetHiddenInGame(true);
	StaticMesh2->SetHiddenInGame(true);
	StaticMesh3->SetHiddenInGame(true);
	WBP_CakeHealth1 = CreateWidget<UUserWidget>(GetGameInstance(), WidgetCakeHealth1);
	WBP_CakeHealth1->AddToViewport();

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
	if (SB_EatCake)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_EatCake, StaticMesh->GetComponentLocation(), GetActorRotation()); //, FRotator::ZeroRotator);


	}
	

	if (CakeHealth == 7)
	{
		StaticMesh->SetHiddenInGame(true);
		StaticMesh1->SetHiddenInGame(false);
		
		WBP_CakeHealth1->RemoveFromParent();
		WBP_CakeHealth2 = CreateWidget<UUserWidget>(GetGameInstance(), WidgetCakeHealth2);
		WBP_CakeHealth2->AddToViewport();
	}

	if (CakeHealth == 4)
	{
		StaticMesh1->SetHiddenInGame(true);
		StaticMesh2->SetHiddenInGame(false);

		WBP_CakeHealth2->RemoveFromParent();
		WBP_CakeHealth3 = CreateWidget<UUserWidget>(GetGameInstance(), WidgetCakeHealth3);
		WBP_CakeHealth3->AddToViewport();
	}

	if (CakeHealth == 1)
	{
		StaticMesh2->SetHiddenInGame(true);
		StaticMesh3->SetHiddenInGame(false);

		WBP_CakeHealth3->RemoveFromParent();
		WBP_CakeHealth4 = CreateWidget<UUserWidget>(GetGameInstance(), WidgetCakeHealth4);
		WBP_CakeHealth4->AddToViewport();
	}

	if (CakeHealth <= 0)
	{
		//if (SB_Lose)
		//{
		//	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Lose, StaticMesh->GetComponentLocation(), GetActorRotation()); //, FRotator::ZeroRotator);


		//}
		GetWorld()->GetFirstPlayerController()->Pause();
		UUserWidget* WGP_GameOver = CreateWidget<UUserWidget>(GetGameInstance(), WidgetClassGameOver);
		WGP_GameOver->AddToViewport();

		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
}
