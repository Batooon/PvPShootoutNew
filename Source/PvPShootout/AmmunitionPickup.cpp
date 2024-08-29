// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmunitionPickup.h"

#include "AmmoSpawner.h"
#include "Core/ShootoutCharacter.h"


AAmmunitionPickup::AAmmunitionPickup()
{
}

void AAmmunitionPickup::BeginPlay()
{
    Super::BeginPlay();
}

void AAmmunitionPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (HasAuthority())
    {
        if (OtherActor && OtherActor != this)
        {
            if (AShootoutCharacter* Character = Cast<AShootoutCharacter>(OtherActor))
            {
                Character->IncreaseAmmoAmount(AmmoAmount);

                AAmmoSpawner* AmmoSpawner = Cast<AAmmoSpawner>(GetOwner());
                if (AmmoSpawner)
                {
                    AmmoSpawner->AmmoPickedUp();
                }

                Destroy();
            }
        }
    }
}
