// Copyright Epic Games, Inc. All Rights Reserved.


#include "PvPShootoutPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "PvPShootoutCharacter.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "SessionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"


void APvPShootoutPlayerController::PointerPressed()
{
    if (const APvPShootoutCharacter* Ch = Cast<APvPShootoutCharacter>(GetPawn()))
    {
        if (Ch->Interaction)
        {
            Ch->Interaction->PressPointerKey(EKeys::LeftMouseButton);
        }
    }
}

void APvPShootoutPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (!IsLocalPlayerController())
    {
        return;
    }

    UGameInstance* gameInstance = GetWorld()->GetGameInstance();
    SessionSubsystem = gameInstance->GetSubsystem<USessionSubsystem>();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
        GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);
    }
}

void APvPShootoutPlayerController::PointerRelease()
{
    if (const APvPShootoutCharacter* Ch = Cast<APvPShootoutCharacter>(GetPawn()))
    {
        if (Ch->Interaction)
        {
            Ch->Interaction->ReleasePointerKey(EKeys::LeftMouseButton);
        }
    }
}

void APvPShootoutPlayerController::TryCreateAndJoinNewSession()
{
    SessionSubsystem->OnCreateSessionCompleteEvent.AddDynamic(this, &ThisClass::JoinSessionIfSucceeded);
    SessionSubsystem->CreateSession(5, false);
}

void APvPShootoutPlayerController::JoinSessionIfSucceeded(bool success)
{
    SessionSubsystem->OnCreateSessionCompleteEvent.RemoveDynamic(this, &ThisClass::JoinSessionIfSucceeded);
    if (success)
    {
        UE_LOG(LogTemp, Display, TEXT("Successfully joined session"));
        UGameplayStatics::OpenLevel(GetWorld(), "Shootout", true, "listen");
    }
}

void APvPShootoutPlayerController::JoinSession(EOnJoinSessionCompleteResult::Type Result)
{
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        SessionSubsystem->TryTravelToCurrentSession();
    }
}

void APvPShootoutPlayerController::SearchSessions()
{
    SessionSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ThisClass::SessionsFound);
    SessionSubsystem->FindSessions(5, false);
}

void APvPShootoutPlayerController::SessionsFound(const TArray<FOnlineSessionSearchResult>& results, bool success)
{
    if (success)
    {
        SessionSubsystem->OnJoinSessionCompleteEvent.AddUObject(this, &ThisClass::JoinSession);
        SessionSubsystem->JoinGameSession(results.Top());
    }
}

void APvPShootoutPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Triggered, this,
                                           &APvPShootoutPlayerController::PointerPressed);
        EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Completed, this,
                                           &APvPShootoutPlayerController::PointerRelease);
    }
}
