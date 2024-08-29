// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/ShootoutCharacter.h"
#include "HealthWidget.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdate, int32, NewHealthAmount);

UCLASS()
class PVPSHOOTOUT_API UHealthWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintAssignable, Category="Health")
    FOnHealthUpdate OnHealthUpdate;

    UFUNCTION()
    void UpdateHealthAmount(int32 NewHealthAmount);

    void SetMaxHealthAmount(int32 MaxHealth);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 MaxHealthAmount;

private:
    AShootoutCharacter* ShootoutCharacter;
};
