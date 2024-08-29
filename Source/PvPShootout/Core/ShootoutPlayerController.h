// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "ShootoutPlayerController.generated.h"

/**
 *
 */
UCLASS()
class PVPSHOOTOUT_API AShootoutPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void OnKilled();

protected:
    virtual void BeginPlay() override;
    // virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* InputMappingContext;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* FireMappingContext;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* HostControlsContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* StartMatchAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* ShowStatsAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* LeaveSessionAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UUserWidget> PlayerStatsClass;

private:
    UFUNCTION(Server, Unreliable)
    void StartMatch();

    UFUNCTION()
    void ShowStats();

    UFUNCTION()
    void LeaveMatch();

    void InitializeBindings();

    UUserWidget* PlayerStatsWidget;

    bool bindingsInitialized;
};
