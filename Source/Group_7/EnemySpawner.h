// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAI.h"
#include "Portal.h"
#include "EnemySpawner.generated.h"



//this script handles the spawning of waves and enemy density

class AEnemyAI;
class APortal;
class AMainCharacter;
UCLASS()
class GROUP_7_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();
	AMainCharacter* MainCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AmoutOfEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int WaveCount; // controlls how many waves to win the game

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int EnemiesDefeated;

	UFUNCTION()
		void SpawnEnemy();

	UFUNCTION()
		void DefeatedEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float MinY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float MaxY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float XPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		TSubclassOf<AEnemyAI> BP_EnemyAI; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		TSubclassOf<APortal> BP_Portal;
	

public:
	float SpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "My variables")
		bool bHasWon;
	int SpawnIndex;
};
