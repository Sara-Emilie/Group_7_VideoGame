// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"


class USphereComponent;
class AMainCharacter;
class AEnemyAI;
class UNiagaraSystem;
class USoundBase;
class ABullet;
UCLASS()
class GROUP_7_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();
	AMainCharacter* MainCharacter;
	ABullet* Bullet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		USphereComponent* DamageSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		TSubclassOf<ABullet> BP_Bullet;
	UFUNCTION()
		void OnReleasedTurret(FVector ForWardVector);
	UFUNCTION()
		void Shoot();
	UFUNCTION()
		void CheckForEnemies();

private:
	FTimerHandle TH_CheckDelay;
	FTimerHandle TH_ShootDelay;
	bool bHasChecked;
	bool bCanShoot;
	float TurretSpeed;
};
