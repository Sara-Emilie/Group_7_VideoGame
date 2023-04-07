// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBox.generated.h"

class USphereComponent;

UCLASS()
class GROUP_7_API APickUpBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		UStaticMeshComponent* PickUpMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
		USphereComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float lifeSpan;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
		void DestroyPickUp();

private:
	float DespawnTimer;
};
