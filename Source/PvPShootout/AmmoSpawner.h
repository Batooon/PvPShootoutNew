// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoSpawner.generated.h"

UCLASS()
class PVPSHOOTOUT_API AAmmoSpawner : public AActor
{
    GENERATED_BODY()

public:
    AAmmoSpawner();

    UPROPERTY(EditAnywhere, Category = Ammo)
    TSubclassOf<AActor> AmmoClass;

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, Category=Ammo)
    float respawnTime;

    UFUNCTION()
    void AmmoPickedUp();

    void SpawnAmmo();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void StartRespawnTimer();

private:
    FTimerHandle respawnTimerHandle;
};
