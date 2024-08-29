// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpawner.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"


APlayerSpawner::APlayerSpawner(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
    SpawnArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SpawnArea->SetCollisionResponseToAllChannels(ECR_Ignore);
    SpawnArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SpawnArea->SetMobility(EComponentMobility::Static);
    SpawnArea->SetupAttachment(GetCapsuleComponent());
    bIsPlayerInArea = false;
}

bool APlayerSpawner::IsSpawnClear()
{
    return !bIsPlayerInArea;
}

void APlayerSpawner::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority())
    {
        SpawnArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
        SpawnArea->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);
    }
}

void APlayerSpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
    bIsPlayerInArea = OtherActor->IsA(ACharacter::StaticClass());
}

void APlayerSpawner::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bIsPlayerInArea = !OtherActor->IsA(ACharacter::StaticClass());
    if (!bIsPlayerInArea)
    {
        OnSpawnCleared.Broadcast();
    }
}
