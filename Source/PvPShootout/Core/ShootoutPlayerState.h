// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShootoutPlayerState.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKillsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPingUpdated);

UCLASS()
class PVPSHOOTOUT_API AShootoutPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AShootoutPlayerState();

    void AddKill();
    void AddDeath();

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void BeginPlay() override;

    UPROPERTY(ReplicatedUsing = OnRep_Kills, BlueprintReadOnly)
    int32 Kills;

    UPROPERTY(ReplicatedUsing = OnRep_Deaths, BlueprintReadOnly)
    int32 Deaths;

    UPROPERTY(BlueprintAssignable)
    FOnDeathsChanged OnDeathsChanged;

    UPROPERTY(BlueprintAssignable)
    FOnKillsChanged OnKillsChanged;

    UPROPERTY(BlueprintAssignable)
    FOnPingUpdated OnPingUpdated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PingUpdateFrequency;

    UFUNCTION()
    void OnRep_Kills();

    UFUNCTION()
    void OnRep_Deaths();

private:
    FTimerHandle PingUpdateTimerHandle;

    void FetchPing();
};
