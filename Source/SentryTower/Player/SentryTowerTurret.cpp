// Copyright (c) 2024 Sentry. All Rights Reserved.


#include "SentryTowerTurret.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASentryTowerTurret::ASentryTowerTurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));

	RootComponent = TurretBase;

	TurretWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretWeapon"));
	TurretWeapon->SetupAttachment(RootComponent);
}

void ASentryTowerTurret::RotateTurret(const FVector& Target)
{
	// Get the direction vector from actor to hit location
	FVector Direction = Target - GetActorLocation();

	// Clamp magnitude to avoid excessive rotation in case of extreme distances
	Direction = Direction.GetSafeNormal();
	

	// Rotate the actor towards the direction vector (assuming actor has a rotator component)
	FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(Direction);
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -40.0f, 0.0f);

	TurretWeapon->SetWorldRotation(NewRotation);
}

// Called when the game starts or when spawned
void ASentryTowerTurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASentryTowerTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

