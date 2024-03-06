// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SentryTowerPawn.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class ASentryTowerTurret;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTowerDiesDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTowerLevelUpDelegate);

/**
 * Tower pawn which fights the enemies
 */
UCLASS()
class SENTRYTOWER_API ASentryTowerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASentryTowerPawn();

	UFUNCTION(BlueprintCallable)
	void RotateTurret(const FVector& RotateTo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GrantExperience(int32 Exp);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> Collision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TowerBase;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TowerMid;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TowerTop;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UChildActorComponent> Turret;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentExp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ExpForCurrentLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ExpForNextLevel = 100;

	UPROPERTY(BlueprintAssignable)
	FOnTowerDiesDelegate OnTowerDies;

	UPROPERTY(BlueprintAssignable)
	FOnTowerDiesDelegate OnTowerLevelUp;
};
