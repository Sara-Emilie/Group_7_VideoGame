// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "Kismet/GameplayStatics.h"
#include "Waypoint.h"
#include "WayPointBox.h"
#include "MyAIController.h"
#include "PickUpBox.h"
#include "Bullet.h"
// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    EnemyMovementSpeed = 500.f;
    GetCharacterMovement()->MaxWalkSpeed = EnemyMovementSpeed;
    EnemyHealth = 2;
    WillDrop = 3;
    DropChance = 0;
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPointBox::StaticClass(), Waypoints);
	MoveToWayPoints();
    DropChance = FMath::RandRange(2, 3);
    
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

    // checks for waypoints and move towards them
    if (EnemyAIController) {
        if (CurrentWayPoint <= Waypoints.Num()) {
            //checks if the current waypoint is less that the next
            //this makes the enemy always move towards the next waypoint
            for (AActor* Waypoint : Waypoints) {
                AWayPointBox* WaypointItr = Cast<AWayPointBox>(Waypoint);
                

                if (WaypointItr) {
						
                    if (WaypointItr->GetWayPointOrder() == CurrentWayPoint) {
                        
                       //the enemy moves to the waypoint and keeps a distance away form the waypoint
                        //they should move close enough to get the cake collider
                        EnemyAIController->MoveToActor(WaypointItr, 500.f, false, true);
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

  
            DestoryTarget();
        }
    }
}

void AEnemyAI::DestoryTarget()
{
    if (DropChance == WillDrop)
    {
        GetWorld()->SpawnActor<APickUpBox>(BP_PickUpBox, GetActorLocation(), GetActorRotation());
        UE_LOG(LogTemp, Warning, TEXT("DropBox"));
    }

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    //AEnemySpawner* Spawner;

    this->Destroy();
}

void AEnemyAI::EnemyTakeDamage()
{
    EnemyHealth--;
    if (EnemyHealth <= 0)
    {

        DestoryTarget();
    }
}

void AEnemyAI::GotTheCake()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    //AEnemySpawner* Spawner;

    this->Destroy();
}





