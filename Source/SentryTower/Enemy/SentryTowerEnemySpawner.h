// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SentryTowerEnemySpawner.generated.h"

class ASentryTowerEnemyBase;

UCLASS()
class SENTRYTOWER_API ASentryTowerEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASentryTowerEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float SpawnDistance = 1000.0f;

	UPROPERTY(EditAnywhere)
	float SpawnFrequency = 1.0f;

	/** Types of enemies that can be spawned */
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ASentryTowerEnemyBase>> EnemyTypes;

private:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UFUNCTION()
	void OnEnemyDies(int32 ExpBonus);
};
