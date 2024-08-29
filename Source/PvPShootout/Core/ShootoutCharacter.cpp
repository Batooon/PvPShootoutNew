// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootoutCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ShootoutGameMode.h"
#include "ShootoutPlayerController.h"
#include "ShootoutPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "PvPShootout/AmmunitionDisplayWidget.h"
#include "PvPShootout/HealthWidget.h"

AShootoutCharacter::AShootoutCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->SetupAttachment(FirstPersonCameraComponent);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;
    Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AShootoutCharacter::CreateAmmoWidget()
{
    if (AmmoWidgetClass)
    {
        if ((AmmoWidget = CreateWidget<UAmmunitionDisplayWidget>(
            GetWorld(), AmmoWidgetClass)))
        {
            AmmoWidget->AddToViewport();
        }
    }
}

void AShootoutCharacter::CreateHealthWidget()
{
    if (HealthWidgetClass)
    {
        if ((HealthWidget = CreateWidget<UHealthWidget>(GetWorld(), HealthWidgetClass)))
        {
            HealthWidget->SetMaxHealthAmount(MaxHealth);
            HealthWidget->AddToViewport();
        }
    }
}

void AShootoutCharacter::CreateWidgets()
{
    CreateAmmoWidget();
    CreateHealthWidget();
}

void AShootoutCharacter::BeginPlay()
{
    Super::BeginPlay();

    WeaponComponent = FindComponentByClass<UTP_WeaponComponent>();
    WeaponComponent->SetCharacter(this);

    if (IsLocallyControlled())
    {
        CreateWidgets();
    }

    if(HasAuthority())
    {
        Health = MaxHealth;
        OnRep_Health();
    }
}

void AShootoutCharacter::BindActions(UEnhancedInputComponent* EnhancedInputComponent)
{
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShootoutCharacter::Move);

    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShootoutCharacter::Look);

    EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AShootoutCharacter::Fire);
}

void AShootoutCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        BindActions(EnhancedInputComponent);
    }
}

void AShootoutCharacter::ProcessDeath()
{
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        PlayerController->UnPossess();
        if(AShootoutPlayerState* ShootoutPlayerState=Cast<AShootoutPlayerState>(PlayerController->PlayerState))
        {
            ShootoutPlayerState->AddDeath();
        }
        ProcessDeathClient();
        Destroy();
        if (AShootoutGameMode* ShootoutGameMode = Cast<AShootoutGameMode>(GetWorld()->GetAuthGameMode()))
        {
            ShootoutGameMode->ProcessPlayerRespawn(PlayerController);
        }
    }
}

void AShootoutCharacter::ProcessDeathClient_Implementation()
{
    HealthWidget -> RemoveFromParent();
    AmmoWidget -> RemoveFromParent();
    HealthWidget = nullptr;
    AmmoWidget = nullptr;
}

bool AShootoutCharacter::Died()
{
    return Health <= 0;
}

void AShootoutCharacter::DecreaseHealth(float DamageAmount)
{
    Health -= DamageAmount;
    OnRep_Health();
}

float AShootoutCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
                                     AActor* DamageCauser)
{
    DecreaseHealth(DamageAmount);
    if (Died())
    {
        ProcessDeath();
        AShootoutPlayerController* ShootoutPlayerController = Cast<AShootoutPlayerController>(EventInstigator);
        ShootoutPlayerController->OnKilled();
    }

    return DamageAmount;
}

void AShootoutCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AShootoutCharacter, AmmoAmount);
    DOREPLIFETIME(AShootoutCharacter, Health);
}

void AShootoutCharacter::OnRep_AmmoAmount()
{
    OnAmmoChanged.Broadcast(AmmoAmount);
}

void AShootoutCharacter::OnRep_Health()
{
    OnHealthChanged.Broadcast(Health);
}

void AShootoutCharacter::IncreaseAmmoAmount(int32 Amount)
{
    AmmoAmount += Amount;
    OnRep_AmmoAmount();
}

void AShootoutCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void AShootoutCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AShootoutCharacter::DecreaseAmmo()
{
    AmmoAmount -= 1;
    OnRep_AmmoAmount();
}

bool AShootoutCharacter::CanFire()
{
    return AmmoAmount > 0;
}

void AShootoutCharacter::Fire_Implementation()
{
    if (CanFire())
    {
        DecreaseAmmo();
        WeaponComponent->Fire();
    }
}
