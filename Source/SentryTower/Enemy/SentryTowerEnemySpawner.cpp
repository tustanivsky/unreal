// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerEnemySpawner.h"
#include "SentryTowerEnemyBase.h"

#include "SentryTower/Player/SentryTowerPawn.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASentryTowerEnemySpawner::ASentryTowerEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASentryTowerEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// Create a timer and start spawning enemies
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASentryTowerEnemySpawner::SpawnEnemy, SpawnFrequency, true);
}

// Called every frame
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

	// Choose a random enemy type
	int32 RandomIndex = FMath::RandRange(0, EnemyTypes.Num() - 1);
	TSubclassOf<ASentryTowerEnemyBase> EnemyType = EnemyTypes[RandomIndex];

	// Get the world and check for validity
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

