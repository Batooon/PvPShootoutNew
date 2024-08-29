// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PvPShootout/TP_WeaponComponent.h"
#include "ShootoutCharacter.generated.h"

class UHealthWidget;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UAmmunitionDisplayWidget;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, NewAmmoAmount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, int32, NewHealthAmount);

UCLASS(config=Game)
class AShootoutCharacter : public ACharacter
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* Mesh1P;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FirstPersonCameraComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

public:
    AShootoutCharacter();
    void CreateAmmoWidget();
    void CreateHealthWidget();
    void CreateWidgets();

protected:
    virtual void BeginPlay() override;
    void BindActions(UEnhancedInputComponent* EnhancedInputComponent);

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* FireAction;

    UPROPERTY(BlueprintAssignable, Category="Ammo")
    FOnAmmoChanged OnAmmoChanged;

    UPROPERTY(BlueprintAssignable, Category="Health")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
    TSubclassOf<UAmmunitionDisplayWidget> AmmoWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
    TSubclassOf<UHealthWidget> HealthWidgetClass;

protected:
    void Move(const FInputActionValue& Value);

    void Look(const FInputActionValue& Value);
    void DecreaseAmmo();
    bool CanFire();

    UFUNCTION(Server, Unreliable)
    void Fire();

    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
    void ProcessDeath();
    UFUNCTION(Client, Unreliable)
    void ProcessDeathClient();
    bool Died();
    void DecreaseHealth(float DamageAmount);

    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_AmmoAmount();

    UFUNCTION()
    void OnRep_Health();

public:
    USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
    UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

    void IncreaseAmmoAmount(int32 Amount);

protected:
    UPROPERTY(ReplicatedUsing = OnRep_AmmoAmount, VisibleAnywhere, BlueprintReadOnly, Category="Ammo")
    int32 AmmoAmount;

    UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category="Health")
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
    int32 MaxHealth;

private:
    UTP_WeaponComponent* WeaponComponent;

    UHealthWidget* HealthWidget;
    UAmmunitionDisplayWidget* AmmoWidget;
};
