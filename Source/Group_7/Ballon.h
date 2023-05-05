// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ballon.generated.h"


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BalloonMesh")
		UStaticMeshComponent* BalloonMesh;


	float ZOfSet;
	float XOfSet;
	float YOfSet;
	float TimePassed;
	float scale;

};
