// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "PvPShootoutProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Core/ShootoutCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
    // Default offset from the character location for projectiles to spawn
    MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::SetCharacter(AShootoutCharacter* Character)
{
    OwnerCharacter = Character;
}


void UTP_WeaponComponent::Fire()
{
    if (OwnerCharacter == nullptr || OwnerCharacter->GetController() == nullptr)
    {
        return;
    }

    // Try and fire a projectile
    if (ProjectileClass != nullptr)
    {
        UWorld* const World = GetWorld();
        if (World != nullptr)
        {
            APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
            const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
            // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
            const FVector SpawnLocation = GetComponentLocation() + SpawnRotation.RotateVector(MuzzleOffset);

            //Set Spawn Collision Handling Override
            FActorSpawnParameters ActorSpawnParams;
            ActorSpawnParams.SpawnCollisionHandlingOverride =
                ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            ActorSpawnParams.Instigator = PlayerController->GetPawn();

            // Spawn the projectile at the muzzle
            APvPShootoutProjectile* projectile = World->SpawnActor<APvPShootoutProjectile>(
                ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
            if (projectile)
            {
                projectile->SetDamageAmount(DamageAmount);
            }
        }
    }

    // Try and play the sound if specified
    if (FireSound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, OwnerCharacter->GetActorLocation());
    }

    // Try and play a firing animation if specified
    if (FireAnimation != nullptr)
    {
        // Get the animation object for the arms mesh
        UAnimInstance* AnimInstance = OwnerCharacter->GetMesh1P()->GetAnimInstance();
        if (AnimInstance != nullptr)
        {
            AnimInstance->Montage_Play(FireAnimation, 1.f);
        }
    }
}

// int32 UTP_WeaponComponent::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
// {
//     if(AActor* OuterActor = Cast<AActor>(OwnerCharacter->GetOuter()))
//     {
//         return OuterActor->GetFunctionCallspace(Function, Stack);
//     }
//
//     return FunctionCallspace::Local;
// }
//
// bool UTP_WeaponComponent::CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms,
//     FFrame* Stack)
// {
//     if(AActor* OuterActor = Cast<AActor>(GetOuter()))
//     {
//         UNetDriver* NetDriver = OuterActor->GetNetDriver();
//         if(NetDriver)
//         {
//             NetDriver->ProcessRemoteFunction(OuterActor, Function, Parms, OutParms, Stack, this);
//             return true;
//         }
//     }
//     return false;
// }
