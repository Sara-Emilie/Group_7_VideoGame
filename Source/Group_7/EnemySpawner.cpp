// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "MainCharacter.h"
#include "Portal.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundBase.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"

// Sets default values
// demon children spawner

AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WaveCount = 0;
	AmoutOfEnemies = 1;
	MaxWaveCount = 10;
	WaveTimer = 5.f;
	WaveCountTime = 3;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnIndex = 0;
	SpawnEnemy();


	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (WaveCount >= MaxWaveCount) {
		WaveCount = MaxWaveCount;
		
	
	}
	// a timer that counts down to spawn a new wave of enemies
	// should also change the timer for balancing
	SpawnTimer += DeltaTime; 
	if (SpawnTimer > WaveTimer && !bHasWon) {
		SpawnEnemy();
		SpawnTimer = 0.f;

	}
}
void AEnemySpawner::SpawnEnemy()
{

	if (bHasWon)
		return; // won't run the code below if you have won
	//add some text for winning the game

	if (WaveCount < MaxWaveCount) {
		for (int i = 0; i < AmoutOfEnemies; i++) {
			AmoutOfEnemies = 3 + WaveCount;

			FVector location;
			float r = FMath::FRandRange(-500.f, 500.f);

			// this stops the enemies from spawning into each other as much
			//spawns enemies in different positions in relation to the
			// different spawnindex (FYI I don't know if index is the right word)
			SpawnIndex++;
			if (SpawnIndex == 1) {
				//float randomSpawnPos;
				location = FVector(GetActorLocation());
				
			}
			else  {
		
				//location = FVector((-5520.f, 7310.f, 180.f));
				location = FVector(GetActorLocation() + FVector(((200.f, r, 10.f))));
				
				SpawnIndex = 0;

			}
			
			
			//spawns the enemy in the world
			int RandomIndex = FMath::RandRange(0, 7);
			AActor* Actor = GetWorld()->SpawnActor<AActor>(BP_UniqueEnemy[RandomIndex].Get(), location, FRotator::ZeroRotator);
			AEnemyAI* EnemyAI = Cast<AEnemyAI>(Actor);
			if (SB_Spawn)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Spawn, location, GetActorRotation()); //, FRotator::ZeroRotator);
			}

			
		}
	}
	if (!(WaveCount >= MaxWaveCount)) {
		WaveCount++;
		WBP_WaveCount = CreateWidget<UUserWidget>(GetGameInstance(), WidgetWaveCount);
		WBP_WaveCount->AddToViewport();

		FTimerHandle TWaveHandle;
		GetWorldTimerManager().SetTimer(TWaveHandle, this, &AEnemySpawner::RemoveWidget, WaveCountTime, false);
	}
	
	

	WaveTimer += 15.f;
	//makes sure the waves don't have too much time inbetween them
	if (WaveTimer > 35.f) {
		WaveTimer = 30.f;

	}
	/*Cast<AMainCharacter>(MainCharacter)->WaveSender(WaveCount);*/   //need to fix to see wavecount
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAI::StaticClass(), Enemies);
	if ((WaveCount >= MaxWaveCount && Enemies.Num() <= 0) || WaveCount >= MaxWaveCount) {
		bHasWon = true;
		if (SB_Win)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Win,GetActorLocation(), GetActorRotation()); //, FRotator::ZeroRotator);
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("YOU WIN!"));

		GetWorld()->GetFirstPlayerController()->Pause();

		WBP_Victory = CreateWidget<UUserWidget>(GetGameInstance(), WidgetClassVictory);
		WBP_Victory->AddToViewport();

		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	}
}

void AEnemySpawner::DefeatedEnemy()
{
	//this code is defunct, remove
	//this code is not being called by anything
	
	EnemiesDefeated++;
	if (EnemiesDefeated >= 3 && WaveCount < 3) {
		WaveCount++;
		EnemiesDefeated = 0;

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("DEAD"));
		//spawn a new wave of enemies
	}
	else if (WaveCount >= 3) {
		// win the game
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Wave count over 3 :D"));
	}
}

void AEnemySpawner::RemoveWidget()
{
	WBP_WaveCount->RemoveFromViewport();
}

