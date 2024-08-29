// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "CreateSessionCallbackProxy.h"
#include "Online/OnlineSessionNames.h"

USessionSubsystem::USessionSubsystem(): CreateSessionCompleteDelegate(
                                            FOnCreateSessionCompleteDelegate::CreateUObject(
                                                this, &ThisClass::OnCreateSessionCompleted)),
                                        StartSessionCompleteDelegate(
                                            FOnStartSessionCompleteDelegate::CreateUObject(
                                                this, &ThisClass::OnStartSessionCompleted)),
                                        EndSessionCompleteDelegate(
                                            FOnEndSessionCompleteDelegate::CreateUObject(
                                                this, &ThisClass::OnEndSessionCompleted)),
                                        DestroySessionCompleteDelegate(
                                            FOnDestroySessionCompleteDelegate::CreateUObject(
                                                this, &ThisClass::OnDestroySessionCompleted)),
                                        FindSessionsCompleteDelegate(
                                            FOnFindSessionsCompleteDelegate::CreateUObject(
                                                this, &ThisClass::OnFindSessionsCompleted)),
                                        JoinSessionCompleteDelegate(
                                            FOnJoinSessionCompleteDelegate::CreateUObject(
                                                this, &ThisClass::OnJoinSessionCompleted))
{
}

void USessionSubsystem::CreateSession(int32 numPublicConnections, bool isLANMatch)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (!sessionInterface.IsValid())
    {
        OnCreateSessionCompleteEvent.Broadcast(false);
        return;
    }

    LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
    LastSessionSettings->NumPrivateConnections = 0;
    LastSessionSettings->NumPublicConnections = numPublicConnections;
    LastSessionSettings->bAllowInvites = true;
    LastSessionSettings->bAllowJoinInProgress = true;
    LastSessionSettings->bAllowJoinViaPresence = true;
    LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
    LastSessionSettings->bIsDedicated = false;
    LastSessionSettings->bUsesPresence = true;
    LastSessionSettings->bIsLANMatch = isLANMatch;
    LastSessionSettings->bShouldAdvertise = true;

    LastSessionSettings->Set(SETTING_MAPNAME, FString("LevelName"), EOnlineDataAdvertisementType::ViaOnlineService);

    CreateSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
        CreateSessionCompleteDelegate);

    const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession,
                                         *LastSessionSettings))
    {
        sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

        OnCreateSessionCompleteEvent.Broadcast(false);
    }
}

void USessionSubsystem::StartSession()
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (!sessionInterface.IsValid())
    {
        OnStartSessionCompleteEvent.Broadcast(false);
        return;
    }

    StartSessionCompleteDelegateHandle = sessionInterface->AddOnStartSessionCompleteDelegate_Handle(
        StartSessionCompleteDelegate);

    if (!sessionInterface->StartSession(NAME_GameSession))
    {
        sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

        OnStartSessionCompleteEvent.Broadcast(false);
    }
}

void USessionSubsystem::EndSession()
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (!sessionInterface.IsValid())
    {
        OnEndSessionCompleteEvent.Broadcast(false);
        return;
    }

    EndSessionCompleteDelegateHandle = sessionInterface->AddOnEndSessionCompleteDelegate_Handle(
        EndSessionCompleteDelegate);

    if (!sessionInterface->EndSession(NAME_GameSession))
    {
        sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);

        OnEndSessionCompleteEvent.Broadcast(false);
    }
}

void USessionSubsystem::DestroySession()
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (!sessionInterface.IsValid())
    {
        OnDestroySessionCompleteEvent.Broadcast(false);
        return;
    }

    DestroySessionCompleteDelegateHandle = sessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
        DestroySessionCompleteDelegate);

    if (!sessionInterface->DestroySession(NAME_GameSession))
    {
        sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

        OnDestroySessionCompleteEvent.Broadcast(false);
    }
}

void USessionSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (!sessionInterface.IsValid())
    {
        OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
        return;
    }

    FindSessionsCompleteDelegateHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
        FindSessionsCompleteDelegate);

    LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
    LastSessionSearch->MaxSearchResults = MaxSearchResults;
    LastSessionSearch->bIsLanQuery = IsLANQuery;

    LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
    {
        sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

        OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
    }
}

void USessionSubsystem::JoinGameSession(const FOnlineSessionSearchResult& SearchResult)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (!sessionInterface.IsValid())
    {
        OnJoinSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
        return;
    }

    JoinSessionCompleteDelegateHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
        JoinSessionCompleteDelegate);

    const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SearchResult))
    {
        sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

        OnJoinSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
    }
}

void USessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (sessionInterface)
    {
        sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
    }

    OnCreateSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (sessionInterface)
    {
        sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
    }

    OnStartSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (sessionInterface)
    {
        sessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
    }

    OnEndSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (sessionInterface)
    {
        sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
    }

    OnDestroySessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::OnFindSessionsCompleted(bool Successful)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (sessionInterface)
    {
        sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
    }

    if (LastSessionSearch->SearchResults.Num() <= 0)
    {
        OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
        return;
    }

    OnFindSessionsCompleteEvent.Broadcast(LastSessionSearch->SearchResults, Successful);
}

void USessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (sessionInterface)
    {
        sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
    }

    OnJoinSessionCompleteEvent.Broadcast(Result);
}

bool USessionSubsystem::TryTravelToCurrentSession()
{
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
    if (!sessionInterface.IsValid())
    {
        return false;
    }

    FString connectString;
    if (!sessionInterface->GetResolvedConnectString(NAME_GameSession, connectString))
    {
        return false;
    }

    APlayerController* playerController = GetWorld()->GetFirstPlayerController();
    playerController->ClientTravel(connectString, TRAVEL_Absolute);
    return true;
}
