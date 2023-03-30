// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "EnemyAI.h"

void AMyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    AEnemyAI* EnemyAI = Cast<AEnemyAI>(GetPawn());

    if (EnemyAI) {
        EnemyAI->MoveToWayPoints();
    }
}
