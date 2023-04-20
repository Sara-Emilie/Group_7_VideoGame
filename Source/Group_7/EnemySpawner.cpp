// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "MainCharacter.h"
#include "Portal.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
// demon children spawner

AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WaveCount = 0;
	AmoutOfEnemies = 1;
	EnemiesDefeated = 0;

	MaxY = 200;
	MinY = 120; //the limit of spawn location

	// should add a timer that shows how long you have left

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

	// a timer that counts down to spawn a new wave of enemies
	// should also change the timer for balancing
	SpawnTimer += DeltaTime; 
	if (SpawnTimer > 7) {
		SpawnEnemy();
		SpawnTimer = 0.f;

	}
}
void AEnemySpawner::SpawnEnemy()
{

	if (bHasWon)
		return; // won't run the code below if you have won
	//add some text for winning the game

	if (WaveCount < 3) {
		for (int i = 0; i < AmoutOfEnemies; i++) {
			AmoutOfEnemies = 3 + WaveCount;

			FVector location;
			float r = FMath::FRandRange(-500, 500);

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
			AActor* Actor = GetWorld()->SpawnActor<AActor>(BP_EnemyAI, location, FRotator::ZeroRotator); 
			AEnemyAI* EnemyAI = Cast<AEnemyAI>(Actor);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Spawned the enemy"));
		}
	}
	WaveCount++;
	/*Cast<AMainCharacter>(MainCharacter)->WaveSender(WaveCount);*/   //need to fix to see wavecount

	if (WaveCount > 3) {
		bHasWon = true;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("YOU WIN!"));

		//CODE needed for l8tr
		// 
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Levels/Victory.Victory'")); // win the game
		//UGameplayStatics::SetGamePaused(GetWorld(), true);
		//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
}

void AEnemySpawner::DefeatedEnemy()
{

	//this code is not being called by anything
	//should probably remove or rework this
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

