// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SentryTowerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TowerBase;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TowerMid;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TowerTop;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Turret;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
};
