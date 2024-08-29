// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsWidget.h"

void UStatsWidget::SetState(APlayerState* NewPlayerState)
{
    State = NewPlayerState;
    OnStateSet.Broadcast();
    //TODO: subscribe to ping updates
}
