// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "Kismet/GameplayStatics.h"
#include "Waypoint.h"
#include "MyAIController.h"
// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), Waypoints);
	MoveToWayPoints();
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyAI::MoveToWayPoints()
{
    AMyAIController* EnemyAIController = Cast<AMyAIController>(GetController());


    if (EnemyAIController) {
        if (CurrentWayPoint <= Waypoints.Num()) {

            for (AActor* Waypoint : Waypoints) {
                AWaypoint* waypointItr = Cast<AWaypoint>(Waypoint);


                if (waypointItr) {

                    if (waypointItr->GetWayPointOrder() == CurrentWayPoint) {

                        EnemyAIController->MoveToActor(waypointItr, 5.f, false);
                        CurrentWayPoint++;
                        break;
                    }

                }


            }


        }
    }



}

