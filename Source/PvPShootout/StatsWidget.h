// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatsWidget.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateSet);

UCLASS()
class PVPSHOOTOUT_API UStatsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetState(APlayerState* State);

protected:
    UPROPERTY(BlueprintReadOnly)
    APlayerState* State;

    UPROPERTY(BlueprintAssignable)
    FOnStateSet OnStateSet;
};
