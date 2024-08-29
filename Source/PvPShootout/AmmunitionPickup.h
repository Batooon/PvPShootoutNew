// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmunitionPickup.generated.h"

UCLASS()
class PVPSHOOTOUT_API AAmmunitionPickup : public AActor
{
    GENERATED_BODY()

public:
    AAmmunitionPickup();

protected:
    virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ammo")
    int32 AmmoAmount;

public:
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
