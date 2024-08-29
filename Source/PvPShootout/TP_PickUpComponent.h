// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PvPShootoutCharacter.h"
#include "TP_PickUpComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PVPSHOOTOUT_API UTP_PickUpComponent : public USphereComponent
{
    GENERATED_BODY()

public:
    UTP_PickUpComponent();

protected:
    /** Called when the game starts */
    virtual void BeginPlay() override;
};
