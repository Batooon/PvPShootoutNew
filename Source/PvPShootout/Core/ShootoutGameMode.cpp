// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootoutGameMode.h"

#include "ShootoutGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PvPShootout/PlayerSpawner.h"

void AShootoutGameMode::FindPlayerSpawner()
{
    if (APlayerSpawner* Spawner = Cast<APlayerSpawner>(
        UGameplayStatics::GetActorOfClass(GetWorld(), APlayerSpawner::StaticClass())))
    {
        PlayerSpawner = Spawner;
    }
}

AShootoutGameMode::AShootoutGameMode() : Super()
{
    FindPlayerSpawner();
}

void AShootoutGameMode::ProcessPlayerRespawn(APlayerController* PlayerController)
{
    if(IsValid(PlayerController))
    {
        RestartPlayer(PlayerController);
    }
}

bool AShootoutGameMode::SpawnOccupied()
{
    return !PlayerSpawner->IsSpawnClear();
}

void AShootoutGameMode::RegisterInWaitSpawnQueue(APlayerController* Player)
{
    PlayersToBeSpawned.Enqueue(Player);
    if (!bIsWaitingToSpawn)
    {
        bIsWaitingToSpawn = true;
        PlayerSpawner->OnSpawnCleared.AddDynamic(this, &ThisClass::SpawnCleared);
    }
}

bool AShootoutGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
    if (!Super::PlayerCanRestart_Implementation(Player))
    {
        return false;
    }

    FindPlayerSpawner();

    if (SpawnOccupied())
    {
        RegisterInWaitSpawnQueue(Player);
        return false;
    }

    return true;
}

void AShootoutGameMode::UpdatePlayersList()
{
    GetGameState<AShootoutGameState>()->PlayersListChanged();
}

void AShootoutGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    UpdatePlayersList();
}

void AShootoutGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    UpdatePlayersList();
}

void AShootoutGameMode::FindValidWaitingPlayer(APlayerController*& WaitingPlayer)
{
    do
    {
        PlayersToBeSpawned.Dequeue(WaitingPlayer);
    }
    while (!IsValid(WaitingPlayer));
}

void AShootoutGameMode::CleanupSpawnEvents()
{
    PlayerSpawner->OnSpawnCleared.RemoveDynamic(this, &ThisClass::SpawnCleared);
    bIsWaitingToSpawn = false;
}

bool AShootoutGameMode::NoPlayersLeftInQueue()
{
    return PlayersToBeSpawned.IsEmpty();
}

void AShootoutGameMode::SpawnCleared()
{
    APlayerController* WaitingPlayer;
    FindValidWaitingPlayer(WaitingPlayer);

    RestartPlayer(WaitingPlayer);
    if (NoPlayersLeftInQueue())
    {
        CleanupSpawnEvents();
    }
}
