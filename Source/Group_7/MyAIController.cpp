// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "EnemyAI.h"

void AMyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{

    //when the enemy has completed its journey to the waypoint, it starts searching for a new waypoint
    AEnemyAI* EnemyAI = Cast<AEnemyAI>(GetPawn());

    if (EnemyAI) {
        EnemyAI->MoveToWayPoints();
    }
}
