// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ballon.generated.h"

class USphereComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class GROUP_7_API ABallon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BalloonMesh")
		UStaticMeshComponent* BalloonMesh;

	//FX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balloon")
		UNiagaraSystem* NS_BalloonPop;


	/** Sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balloon")
		USoundBase* SB_BalloonPop;

	void BalloonPop();


	float ZOfSet;
	float XOfSet;
	float YOfSet;
	float TimePassed;
	float scale;

};
