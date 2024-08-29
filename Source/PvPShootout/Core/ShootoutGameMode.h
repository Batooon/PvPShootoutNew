// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShootoutGameMode.generated.h"

class APlayerSpawner;
/**
 *
 */
UCLASS()
class PVPSHOOTOUT_API AShootoutGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    void FindPlayerSpawner();
    AShootoutGameMode();

    void ProcessPlayerRespawn(APlayerController* PlayerController);
    bool SpawnOccupied();
    void RegisterInWaitSpawnQueue(APlayerController* Player);

protected:
    virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
    void UpdatePlayersList();

    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;
    void FindValidWaitingPlayer(APlayerController*& WaitingPlayer);
    void CleanupSpawnEvents();
    bool NoPlayersLeftInQueue();

    UPROPERTY(BlueprintReadWrite)
    APlayerSpawner* PlayerSpawner;

    TQueue<APlayerController*> PlayersToBeSpawned;

    UFUNCTION()
    void SpawnCleared();

    bool bIsWaitingToSpawn;
};
