// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootoutPlayerState.h"

#include "Net/UnrealNetwork.h"

AShootoutPlayerState::AShootoutPlayerState() : Super(), Kills(0), Deaths(0)
{
    SetShouldUpdateReplicatedPing(false);
}

void AShootoutPlayerState::AddKill()
{
    if (HasAuthority())
    {
        Kills++;
        OnRep_Kills();
    }
}

void AShootoutPlayerState::AddDeath()
{
    if (HasAuthority())
    {
        Deaths++;
        OnRep_Deaths();
    }
}

void AShootoutPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AShootoutPlayerState, Kills);
    DOREPLIFETIME(AShootoutPlayerState, Deaths);
}

void AShootoutPlayerState::BeginPlay()
{
    Super::BeginPlay();

    FetchPing();
    GetWorld()->GetTimerManager().SetTimer(PingUpdateTimerHandle, this, &ThisClass::FetchPing, PingUpdateFrequency,
                                           true);
}

void AShootoutPlayerState::OnRep_Kills()
{
    OnKillsChanged.Broadcast();
}

void AShootoutPlayerState::OnRep_Deaths()
{
    OnDeathsChanged.Broadcast();
}

void AShootoutPlayerState::FetchPing()
{
    RecalculateAvgPing();
    OnPingUpdated.Broadcast();
}
