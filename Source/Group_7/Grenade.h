// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

class USphereComponent;
class AMainCharacter;
class AEnemyAI;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class GROUP_7_API AGrenade : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AGrenade();
	AMainCharacter* MainCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


		/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		USphereComponent* DamageSphere;


		/** Public Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		float FuseTime;


		/** FX - Effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		UNiagaraSystem* NS_Explosion;


		/** Sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		USoundBase* SB_Explosion;


		/** Public Functions */
	UFUNCTION()
		void OnReleased(FVector ForWardVector);

	UFUNCTION()
		void Explode();
	UFUNCTION()
		void RunningSpeed(bool Running);


		/** Private Variables*/
private:
	float LifeSpan;
	float MaxGrenadeSpeed;
	float GrenadeSpeed;

};
