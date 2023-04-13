// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAI.generated.h"

class APickUpBox;

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
	void MoveToWayPoints();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
		int CurrentWayPoint;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		int EnemyHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		TSubclassOf<APickUpBox> BP_PickUpBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float EnemyMovementSpeed;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DestoryTarget();

	UFUNCTION()
		void TakeDamage();



	TArray<AActor*> Waypoints;
private:
	float DropChance;
	float WillDrop;

};
