// Copyright Epic Games, Inc. All Rights Reserved.

#include "PvPShootoutProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

APvPShootoutProjectile::APvPShootoutProjectile()
{
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(8.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &APvPShootoutProjectile::OnHit);
    // set up a notification for when this component hits something blocking

    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    RootComponent = CollisionComp;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;

    InitialLifeSpan = .0f;
}

void APvPShootoutProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                   FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Display, TEXT("OnHit"));
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Display, TEXT("OnHitAuthority"));
        if (OtherActor && OtherActor != this && OtherActor != GetOwner())
        {
            UE_LOG(LogTemp, Display, TEXT("Applying point damage"));
            UE_LOG(LogTemp, Display, TEXT("%s"), *FString(OtherActor->GetClass()->GetName()));
            UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetInstigatorController(), this,
                                          UDamageType::StaticClass());

            Destroy();
        }
    }
}

void APvPShootoutProjectile::SetDamageAmount(float Damage)
{
    DamageAmount = Damage;
}
