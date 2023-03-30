// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
// demon childen spawner
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WaveCount = 0;
	AmoutOfEnemies = 3;
	EnemiesDefeated = 0;

	MaxY = 200;
	MinY = 120; //the limit of spawn location

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemy();
	
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	if (WaveCount < 3) {
		for (int i = 0; i < AmoutOfEnemies; i++) {
			AmoutOfEnemies = 3 + WaveCount;

			// CODE FOR SPAWNING ENEMIES
			// 
			FVector location = FVector((GetActorLocation()));
			AActor* Actor = GetWorld()->SpawnActor<AActor>(BP_Enemy, location, FRotator::ZeroRotator); 
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Spawned the enemy"));
		}
	}
	WaveCount++;
	if (WaveCount > 3) {
		bHasWon = true;
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Levels/Victory.Victory'")); // win the game
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("YOU WIN!"));
		//UGameplayStatics::SetGamePaused(GetWorld(), true);
		//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
}

void AEnemySpawner::DefeatedEnemy()
{
	EnemiesDefeated++;
	if (EnemiesDefeated >= 3 && WaveCount < 3) {
		WaveCount++;
		EnemiesDefeated = 0;

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("DEAD"));
		//spawn a new wave of enemies
	}
	else if (WaveCount >= 3) {
		// win the game, should add a debug message for this
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Wave count over 3 :D"));
	}
}

