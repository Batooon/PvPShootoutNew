// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootoutGameState.h"

#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"

void AShootoutGameState::FindAllAmmoSpawners()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAmmoSpawner::StaticClass(), FoundActors);

    AmmoSpawners.Empty();
    for (AActor* actor : FoundActors)
    {
        AAmmoSpawner* Spawner = Cast<AAmmoSpawner>(actor);
        if (Spawner)
        {
            AmmoSpawners.Add(Spawner);
        }
    }
}

AShootoutGameState::AShootoutGameState()
{
    if (GetWorld())
    {
        FindAllAmmoSpawners();
    }
}

void AShootoutGameState::PlayersListChanged_Implementation()
{
    OnPlayersListChanged.Broadcast();
}

void AShootoutGameState::SpawnAmmoAtRandom()
{
    Algo::RandomShuffle(AmmoSpawners);

    AmmoSpawnsOnMap = FMath::Min(AmmoSpawnsOnMap, AmmoSpawners.Num());

    for (int32 i = 0; i < AmmoSpawnsOnMap; i++)
    {
        AAmmoSpawner* Spawner = AmmoSpawners[i];
        Spawner->SpawnAmmo();
    }
}

bool AShootoutGameState::CanSpawnAnyAmmo()
{
    return AmmoSpawners.Num() > 0 && AmmoSpawnsOnMap > 0;
}

void AShootoutGameState::StartMatch()
{
    if (CanSpawnAnyAmmo())
    {
        SpawnAmmoAtRandom();
    }
}
