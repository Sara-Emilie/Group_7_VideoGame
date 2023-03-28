// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

class USphereComponent;
class AMainCharacter;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class GROUP_7_API AGrenade : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		USphereComponent* DamageSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		float FuseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		UNiagaraSystem* NS_Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		USoundBase* SB_Explosion;

	UFUNCTION()
		void OnReleased(FVector ForWardVector);


	UFUNCTION()
		void Exsplode();

private:
	float LifeSpan;

};
