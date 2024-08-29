// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatsWidget.h"

#include "StatsWidget.h"
#include "Core/ShootoutGameState.h"

class AShootoutGameState;

void UPlayerStatsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    AShootoutGameState* ShootoutGameState = Cast<AShootoutGameState>(GetWorld()->GetGameState());
    InitStats();
    ShootoutGameState->OnPlayersListChanged.AddDynamic(this, &ThisClass::InitStats);
}

void UPlayerStatsWidget::InitStats()
{
    StatsContainer->ClearChildren();

    AShootoutGameState* ShootoutGameState = Cast<AShootoutGameState>(GetWorld()->GetGameState());
    for (TObjectPtr<APlayerState> player : ShootoutGameState->PlayerArray)
    {
        UStatsWidget* PlayerStat = CreateWidget<UStatsWidget>(GetWorld(), StatClass);
        PlayerStat->SetState(player);
        PlayerStat->AddToViewport();
        StatsContainer->AddChild(PlayerStat);
    }
}
