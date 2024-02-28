// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerEnemyBase.h"

// Sets default values
ASentryTowerEnemyBase::ASentryTowerEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASentryTowerEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASentryTowerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASentryTowerEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

