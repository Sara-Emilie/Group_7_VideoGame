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
class USoundBase;


UCLASS()
class GROUP_7_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	AMainCharacter* MainCharacter;

		/** Sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SB_Spawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_Win;

	/** Refrence to WidgetBp */
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Widget")
		TSubclassOf<class UUserWidget> WidgetClassVictory;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Widget")
		TSubclassOf<class UUserWidget> WidgetWaveCount;



	UUserWidget* WBP_Victory;
	UUserWidget* WBP_WaveCount;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		/** Public Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AmoutOfEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int EnemiesDefeated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int EnemiesLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int WaveCount; // counts current wave

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxWaveCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WaveTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int StopBigKid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int BigKidSpawnIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float XPosition;

	float SpawnTimer;

	int SpawnIndex;

		/** Arrays */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AEnemyAI>> BP_UniqueEnemy;
		/** Reference to other Blueprints of Actors*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AEnemyAI>> BP_BossEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		TSubclassOf<AEnemyAI> BP_EnemyAI;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		TSubclassOf<APortal> BP_Portal;


		/** Functions */
	UFUNCTION()
		void SpawnEnemy();
	UFUNCTION()
		void DelaySpawnEnemy();
	UFUNCTION()
		void DefeatedEnemy();
	UFUNCTION()
		void WinTheGame();

	void RemoveWidget();

		/** Booleans */
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "My variables")
		bool bHasWon;
	int WaveCountTime;
private:
	FTimerHandle TH_SpawnDelay;
	bool bCanSpawn;
};
