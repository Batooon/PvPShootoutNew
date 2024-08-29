// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "PlayerStatsWidget.generated.h"

class UStatsWidget;
/**
 *
 */
UCLASS()
class PVPSHOOTOUT_API UPlayerStatsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UStatsWidget> StatClass;

private:
    UPROPERTY(meta=(BindWidget))
    UVerticalBox* StatsContainer;

    UFUNCTION()
    void InitStats();
};
