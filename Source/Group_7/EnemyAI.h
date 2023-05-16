// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAI.generated.h"

class APickUpBox;
class USphereComponent;
class USoundBase;
class UNiagaraSystem;
UCLASS()
class GROUP_7_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


		/** Reference to other */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		TSubclassOf<APickUpBox> BP_PickUpBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USphereComponent* DeatheffectSphere;

		/** Public Variables */
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
		int CurrentWayPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int EnemyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float EnemyMovementSpeed;


		/** Sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_Death;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_laugh01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_laugh02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_laugh03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_EnemyHit;

	/**FX's*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		UNiagaraSystem* NS_Death;

		/** Functions */
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DestoryTarget();

	UFUNCTION()
		void EnemyTakeDamage();

	UFUNCTION()
		void GotTheCake();

	void MoveToWayPoints(); //Makes it move to the waypoints

		/** Arrays */
	TArray<AActor*> Waypoints;


		/** Private Variables*/
private:
	float DropChance;

	float WillDrop;

	int WaypointChance;

	int VoiceChance;
};
