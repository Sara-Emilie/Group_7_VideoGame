// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Waypoint.generated.h"

/**
 * 
 */
UCLASS()
class GROUP_7_API AWaypoint : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	int GetWayPointOrder();

private:
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
		int WayPointOrder;
};
