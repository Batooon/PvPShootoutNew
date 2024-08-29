// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerSpawner.generated.h"

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnCleared);

UCLASS()
class PVPSHOOTOUT_API APlayerSpawner : public APlayerStart
{
    GENERATED_BODY()

public:
    APlayerSpawner(const FObjectInitializer& ObjectInitializer);

    bool IsSpawnClear();

    FOnSpawnCleared OnSpawnCleared;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    APlayerStart* StartLocation;

private:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* SpawnArea;

    bool bIsPlayerInArea;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex);
};
