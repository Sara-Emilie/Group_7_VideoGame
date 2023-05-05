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

		/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUpMesh")
		UStaticMeshComponent* PickUpMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUpMesh")
		USphereComponent* Collider;


		/** Public Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Variables")
		float lifeSpan;


		/** Public Functions */
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DestroyPickUp();


		/** Sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_PickUp;

	
		/** Private Variables*/
private:
	float DespawnTimer;
	float ZOfSet;
	float TimePassed;
};
