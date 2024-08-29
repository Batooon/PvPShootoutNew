// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class AShootoutCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PVPSHOOTOUT_API UTP_WeaponComponent : public USkeletalMeshComponent
{
    GENERATED_BODY()

public:
    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=Projectile)
    TSubclassOf<class APvPShootoutProjectile> ProjectileClass;

    /** Sound to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
    USoundBase* FireSound;

    /** AnimMontage to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    UAnimMontage* FireAnimation;

    /** Gun muzzle's offset from the characters location */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
    FVector MuzzleOffset;

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    class UInputMappingContext* FireMappingContext;

    /** Fire Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    class UInputAction* FireAction;

    /** Sets default values for this component's properties */
    UTP_WeaponComponent();

    void SetCharacter(AShootoutCharacter* Character);

    /** Make the weapon Fire a Projectile */
    void Fire();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount;

    AShootoutCharacter* OwnerCharacter;
};
