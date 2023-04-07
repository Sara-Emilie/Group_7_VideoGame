// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "Kismet/GameplayStatics.h"
#include "Waypoint.h"
#include "MyAIController.h"
#include "PickUpBox.h"
#include "Bullet.h"
// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    EnemyHealth = 2;
    WillDrop = 3;
    DropChance = 3;
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), Waypoints);
	MoveToWayPoints();
    /*DropChance = FMath::RandRange(3, 3);*/
    
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

void AEnemyAI::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA<ABullet>()) {
        Cast<ABullet>(OtherActor)->DestroyBullet();

        EnemyHealth--;

        if (EnemyHealth <= 0)
        {
           //Spawn Dropbox/Grenade box 
            if(DropChance == WillDrop)
            {
                GetWorld()->SpawnActor<APickUpBox>(BP_PickUpBox, GetActorLocation(), GetActorRotation());
                UE_LOG(LogTemp, Warning, TEXT("DropBox"));
            }
            DestoryTarget();
        }
    }
}

void AEnemyAI::DestoryTarget()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    //AEnemySpawner* Spawner;

    this->Destroy();
}

void AEnemyAI::TakeDamage()
{
    EnemyHealth--;
    if (EnemyHealth <= 0)
    {
        DestoryTarget();
    }
}

