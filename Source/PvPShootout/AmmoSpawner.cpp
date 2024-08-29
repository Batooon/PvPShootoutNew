// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoSpawner.h"


// Sets default values
AAmmoSpawner::AAmmoSpawner()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAmmoSpawner::BeginPlay()
{
    Super::BeginPlay();
}

void AAmmoSpawner::SpawnAmmo()
{
    if (AmmoClass)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        SpawnParameters.Instigator = GetInstigator();

        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();

        GetWorld()->SpawnActor<AActor>(AmmoClass, SpawnLocation, SpawnRotation, SpawnParameters);
    }
}

void AAmmoSpawner::AmmoPickedUp()
{
    StartRespawnTimer();
}

void AAmmoSpawner::StartRespawnTimer()
{
    GetWorld()->GetTimerManager().SetTimer(respawnTimerHandle, this, &ThisClass::SpawnAmmo, respawnTime, false);
}

void AAmmoSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
