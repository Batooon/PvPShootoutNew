// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// #include "PlayerHUD.h"
#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "SessionSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PvPShootoutPlayerController.generated.h"

class USessionSubsystem;
class UInputMappingContext;
class FOnlineSessionSearchResult;

/**
 *
 */
UCLASS()
class PVPSHOOTOUT_API APvPShootoutPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
    UInputAction* Interact;

    void PointerPressed();
    void TryCreateAndJoinNewSession();
    void SearchSessions();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* InputMappingContext;


    virtual void BeginPlay() override;
    void PointerRelease();
    UFUNCTION()
    void JoinSessionIfSucceeded(bool success);
    void JoinSession(EOnJoinSessionCompleteResult::Type Result);
    void SessionsFound(const TArray<FOnlineSessionSearchResult>& results, bool success);
    virtual void SetupInputComponent() override;

    TWeakObjectPtr<USessionSubsystem> SessionSubsystem;
};
