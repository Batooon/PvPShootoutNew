// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"

void UHealthWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ShootoutCharacter = Cast<AShootoutCharacter>(GetOwningPlayerPawn());

    if (ShootoutCharacter)
    {
        ShootoutCharacter->OnHealthChanged.AddDynamic(this, &ThisClass::UpdateHealthAmount);
    }
}

void UHealthWidget::UpdateHealthAmount(int32 NewHealthAmount)
{
    OnHealthUpdate.Broadcast(NewHealthAmount);
}

void UHealthWidget::SetMaxHealthAmount(int32 MaxHealth)
{
    MaxHealthAmount = MaxHealth;
}
