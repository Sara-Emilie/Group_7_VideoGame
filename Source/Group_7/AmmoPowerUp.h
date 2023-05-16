// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPowerUp.generated.h"

class USphereComponent;

UCLASS()
class GROUP_7_API AAmmoPowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		float TimePassed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		float Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My variables")
		float ZOffset;

	/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUpMesh")
		UStaticMeshComponent* PickUpMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUpMesh")
		USphereComponent* Collider;

	/** Public Functions */
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DestroyPickUp();


};
