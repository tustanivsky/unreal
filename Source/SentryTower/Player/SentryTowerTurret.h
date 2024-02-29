// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SentryTowerTurret.generated.h"

UCLASS()
class SENTRYTOWER_API ASentryTowerTurret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASentryTowerTurret();

	UFUNCTION(BlueprintCallable)
	void RotateTurret(const FVector& Target);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TurretBase;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TurretWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
