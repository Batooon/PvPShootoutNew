// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/ShootoutCharacter.h"
#include "AmmunitionDisplayWidget.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoUpdated, int32, NewAmmoAmount);


UCLASS()
class PVPSHOOTOUT_API UAmmunitionDisplayWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintAssignable, Category="Ammo")
    FOnAmmoUpdated OnAmmoUpdated;

    UFUNCTION()
    void UpdateAmmoAmount(int32 NewAmmoAmount);

private:
    AShootoutCharacter* ShootoutCharacter;
};
