// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "WayPointBox.h"
#include "MyAIController.h"
#include "PickUpBox.h"
#include "Bullet.h"
#include "Sound/SoundBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    EnemyMovementSpeed = 500.f;
    GetCharacterMovement()->MaxWalkSpeed = EnemyMovementSpeed;

    DeatheffectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

    EnemyHealth = 2;
    WillDrop = 3;
    DropChance = 0;
    WaypointChance = FMath::RandRange(0, 2);
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPointBox::StaticClass(), Waypoints);
	MoveToWayPoints();
    DropChance = FMath::RandRange(1, 5);
    CurrentWayPoint = WaypointChance;
    VoiceChance = FMath::RandRange(0, 60);
    if (VoiceChance <= 40) {
        if (VoiceChance == 10) {
            if (SB_laugh01)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_laugh01, GetActorLocation(), GetActorRotation()); //, FRotator::ZeroRotator);
            }
        }
        if (VoiceChance == 20) {
            if (SB_laugh02)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_laugh02, GetActorLocation(), GetActorRotation()); //, FRotator::ZeroRotator);
            }

        }
        if (VoiceChance == 30) {
            if (SB_laugh03)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_laugh03, GetActorLocation(), GetActorRotation()); //, FRotator::ZeroRotator);
            }

        }
    }
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (GetVelocity().Y <= 0 && GetVelocity().X <= 0) {
        MoveToWayPoints();
   }
    if (CurrentWayPoint >= 11) {
        CurrentWayPoint = 8;
        MoveToWayPoints();
    }
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
                      
                        int WaypointRandomizer = FMath::RandRange(1, 3);
                        CurrentWayPoint += WaypointRandomizer;
                        break;
                    }

                }

            }

        }
    }



}


void AEnemyAI::DestoryTarget()
{
    //Spawn Dropbox/Grenade box 
    if (DropChance == WillDrop)
    {
        GetWorld()->SpawnActor<APickUpBox>(BP_PickUpBox, GetActorLocation(), GetActorRotation());
    }
    if (SB_Death)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Death, GetActorLocation(), GetActorRotation()); //, FRotator::ZeroRotator);
    }
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    this->Destroy();
}

void AEnemyAI::EnemyTakeDamage()
{
    EnemyHealth--;
    if (SB_EnemyHit)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_EnemyHit, GetActorLocation(), GetActorRotation()); //, FRotator::ZeroRotator);
    }
    if (EnemyHealth <= 0)
    {
        DeatheffectSphere->SetWorldLocation(GetActorLocation());
        if (NS_Death)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Death, GetActorLocation());
        }

        DestoryTarget();
    }
}

void AEnemyAI::GotTheCake()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    this->Destroy();
}





