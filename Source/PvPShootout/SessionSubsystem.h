// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionSubsystem.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSessionCompleted, bool, Successful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartSessionCompleted, bool, Successful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndSessionCompleted, bool, Successful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroySessionCompleted, bool, Successful);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFindSessionsCompleted, const TArray<FOnlineSessionSearchResult>& SessionResults,
                                     bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnJoinSessionCompleted, EOnJoinSessionCompleteResult::Type Result);

UCLASS()
class PVPSHOOTOUT_API USessionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    USessionSubsystem();

    void CreateSession(int32 numPublicConnections, bool isLANMatch);
    void StartSession();
    void EndSession();
    void DestroySession();
    void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
    void JoinGameSession(const FOnlineSessionSearchResult& SearchResult);
    bool TryTravelToCurrentSession();

    FOnCreateSessionCompleted OnCreateSessionCompleteEvent;
    FOnStartSessionCompleted OnStartSessionCompleteEvent;
    FOnEndSessionCompleted OnEndSessionCompleteEvent;
    FOnDestroySessionCompleted OnDestroySessionCompleteEvent;
    FOnFindSessionsCompleted OnFindSessionsCompleteEvent;
    FOnJoinSessionCompleted OnJoinSessionCompleteEvent;

protected:
    void OnCreateSessionCompleted(FName SessionName, bool Successful);
    void OnStartSessionCompleted(FName SessionName, bool Successful);
    void OnEndSessionCompleted(FName SessionName, bool Successful);
    void OnDestroySessionCompleted(FName SessionName, bool Successful);
    void OnFindSessionsCompleted(bool Successful);
    void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
    FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
    FDelegateHandle CreateSessionCompleteDelegateHandle;

    FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
    FDelegateHandle StartSessionCompleteDelegateHandle;

    FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
    FDelegateHandle EndSessionCompleteDelegateHandle;

    FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
    FDelegateHandle DestroySessionCompleteDelegateHandle;

    FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
    FDelegateHandle FindSessionsCompleteDelegateHandle;

    FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
    FDelegateHandle JoinSessionCompleteDelegateHandle;

    TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
    TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
};
