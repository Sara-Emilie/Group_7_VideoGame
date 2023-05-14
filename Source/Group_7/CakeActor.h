// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CakeActor.generated.h"


class USphereComponent;
class AMainCharacter;

UCLASS()
class GROUP_7_API ACakeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACakeActor();

	AMainCharacter* MainCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	
	virtual void Tick(float DeltaTime) override;

		/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh3;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		USphereComponent* Collider;

		/**  Public Variables */
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Cake variables")
		float CakeHealth;

		/** Booleans */
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Cake variables")
		bool bCakeDestoryed;

		/** Sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_Lose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound & Effects")
		USoundBase* SB_EatCake;

		/** Functions */
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DamageCake();


		/** Reference to widgets */
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		TSubclassOf<class UUserWidget> WidgetClassGameOver; 

};
