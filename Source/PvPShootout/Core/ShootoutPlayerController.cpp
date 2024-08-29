// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootoutPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShootoutCharacter.h"
#include "ShootoutGameState.h"
#include "ShootoutPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PvPShootout/SessionSubsystem.h"

class AShootoutPlayerState;
class USessionSubsystem;

void AShootoutPlayerController::OnKilled()
{
    if(AShootoutPlayerState* ShootoutPlayerState = Cast<AShootoutPlayerState>(PlayerState))
    {
        ShootoutPlayerState->AddKill();
    }
}

void AShootoutPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if(IsLocalPlayerController())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
                UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
            Subsystem->AddMappingContext(FireMappingContext, 1);
            if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
            {
                EnhancedInputComponent->BindAction(ShowStatsAction, ETriggerEvent::Triggered, this,
                                                   &ThisClass::ShowStats);
                EnhancedInputComponent->BindAction(LeaveSessionAction, ETriggerEvent::Triggered, this,
                                                   &ThisClass::LeaveMatch);
            }
        }

        if (PlayerStatsClass)
        {
            if ((PlayerStatsWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerStatsClass)))
            {
                PlayerStatsWidget->AddToViewport();
                PlayerStatsWidget->SetVisibility(ESlateVisibility::Hidden);
            }
        }

        if (HasAuthority())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
                UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
            {
                Subsystem->AddMappingContext(HostControlsContext, 0);
                if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
                {
                    EnhancedInputComponent->BindAction(StartMatchAction, ETriggerEvent::Triggered, this,
                                                       &ThisClass::StartMatch);
                }
            }
        }
    }
}

// void AShootoutPlayerController::OnPossess(APawn* InPawn)
// {
//     Super::OnPossess(InPawn);
//     if(IsLocalPlayerController())
//     {
//         if(!bindingsInitialized && Cast<AShootoutCharacter>(InPawn))
//         {
//             bindingsInitialized = true;
//             InitializeBindings();
//         }
//     }
// }

void AShootoutPlayerController::ShowStats()
{
    if (PlayerStatsWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        PlayerStatsWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        PlayerStatsWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AShootoutPlayerController::LeaveMatch()
{
    UGameInstance* gameInstance = GetWorld()->GetGameInstance();
    USessionSubsystem* SessionSubsystem = gameInstance->GetSubsystem<USessionSubsystem>();
    UGameplayStatics::OpenLevel(this, "FirstPersonMap");
    SessionSubsystem->DestroySession();
}

void AShootoutPlayerController::InitializeBindings()
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
        UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);
        Subsystem->AddMappingContext(FireMappingContext, 1);
        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
        {
            EnhancedInputComponent->BindAction(ShowStatsAction, ETriggerEvent::Triggered, this,
                                               &ThisClass::ShowStats);
            EnhancedInputComponent->BindAction(LeaveSessionAction, ETriggerEvent::Triggered, this,
                                               &ThisClass::LeaveMatch);
        }
    }

    if (PlayerStatsClass)
    {
        if ((PlayerStatsWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerStatsClass)))
        {
            PlayerStatsWidget->AddToViewport();
            PlayerStatsWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (HasAuthority())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
            UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(HostControlsContext, 0);
            if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
            {
                EnhancedInputComponent->BindAction(StartMatchAction, ETriggerEvent::Triggered, this,
                                                   &ThisClass::StartMatch);
            }
        }
    }
}

void AShootoutPlayerController::StartMatch_Implementation()
{
    if (AShootoutGameState* ShootoutGameState = Cast<AShootoutGameState>(GetWorld()->GetGameState()))
    {
        ShootoutGameState->StartMatch();
    }
}
