// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SentryTowerTurret.generated.h"

class ASentryTowerProjectile;
class USoundBase;

UCLASS()
class SENTRYTOWER_API ASentryTowerTurret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASentryTowerTurret();

	UFUNCTION(BlueprintCallable)
	void RotateTurret(const FVector& Target);

	UFUNCTION(BlueprintPure)
	bool IsFacingTarget(const FVector& Target, float Tolerance) const;

	UFUNCTION()
	void Shoot(AActor* TargetActor, const FVector& TargetLocation);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TurretBase;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TurretWeapon;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> ProjectileSocket;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ShootSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASentryTowerProjectile> ProjectileType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeed = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackCooldown = 0.3f;

	float LastShotTime = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
