// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SentryTowerEnemyBase.generated.h"

/**
 * Enemy pawn which attacks the tower
 */
UCLASS()
class SENTRYTOWER_API ASentryTowerEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASentryTowerEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Body;
};
