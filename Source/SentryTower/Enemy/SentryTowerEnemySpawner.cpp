// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerEnemySpawner.h"

#include "SentryTowerEnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "SentryTower/Player/SentryTowerPawn.h"

ASentryTowerEnemySpawner::ASentryTowerEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASentryTowerEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASentryTowerEnemySpawner::SpawnEnemy, SpawnFrequency, true);
}

void ASentryTowerEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASentryTowerEnemySpawner::SpawnEnemy()
{
	if (EnemyTypes.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ASentryTowerEnemySpawner: No enemy types defined!"));
		return;
	}

	int32 RandomIndex = FMath::RandRange(0, EnemyTypes.Num() - 1);
	TSubclassOf<ASentryTowerEnemyBase> EnemyType = EnemyTypes[RandomIndex];

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ASentryTowerEnemySpawner: Unable to get world!"));
		return;
	}

	FVector SpawnerLocation = GetActorLocation();
	FRotator SpawnerRotation = GetActorRotation();

	float Angle = FMath::RandRange(0.0f, 2.0f * PI);
	FVector SpawnDirection = SpawnerRotation.RotateVector(FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f));

	FVector SpawnLocation = SpawnerLocation + SpawnDirection * SpawnDistance;

	FVector LookAtDirection = SpawnerLocation - SpawnLocation;

	FRotator EnemyRotation = LookAtDirection.Rotation();

	ASentryTowerEnemyBase* SpawnedEnemy = Cast<ASentryTowerEnemyBase>(World->SpawnActor(EnemyType, &SpawnLocation, &EnemyRotation));
	if (!SpawnedEnemy)
	{
		UE_LOG(LogTemp, Error, TEXT("ASentryTowerEnemySpawner: Failed to spawn enemy!"));
		return;
	}

	SpawnedEnemy->OnEnemyDies.AddDynamic(this, &ASentryTowerEnemySpawner::OnEnemyDies);
}

void ASentryTowerEnemySpawner::OnEnemyDies(int32 ExpBonus)
{
	auto TowerPawn = Cast<ASentryTowerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!TowerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASentryTowerEnemySpawner: Can't get tower pawn!"));
		return;
	}

	TowerPawn->GrantExperience(ExpBonus);
}

