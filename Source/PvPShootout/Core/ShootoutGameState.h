// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PvPShootout/AmmoSpawner.h"
#include "ShootoutGameState.generated.h"

class APlayerSpawner;
/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayersListChanged);

UCLASS()
class PVPSHOOTOUT_API AShootoutGameState : public AGameState
{
    GENERATED_BODY()

public:
    void FindAllAmmoSpawners();
    AShootoutGameState();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ShootoutGameState")
    FOnPlayersListChanged OnPlayersListChanged;

    UFUNCTION(NetMulticast, Unreliable)
    void PlayersListChanged();

    void SpawnAmmoAtRandom();
    bool CanSpawnAnyAmmo();
    void StartMatch();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ammo")
    TArray<AAmmoSpawner*> AmmoSpawners;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 AmmoSpawnsOnMap;

    UPROPERTY(BlueprintReadWrite)
    APlayerSpawner* PlayerSpawner;
};
