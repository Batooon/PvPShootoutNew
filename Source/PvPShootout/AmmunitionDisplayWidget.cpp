// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmunitionDisplayWidget.h"

void UAmmunitionDisplayWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ShootoutCharacter = Cast<AShootoutCharacter>(GetOwningPlayerPawn());

    if (ShootoutCharacter)
    {
        ShootoutCharacter->OnAmmoChanged.AddDynamic(this, &ThisClass::UpdateAmmoAmount);
    }
}

void UAmmunitionDisplayWidget::UpdateAmmoAmount(int32 NewAmmoAmount)
{
    OnAmmoUpdated.Broadcast(NewAmmoAmount);
}
