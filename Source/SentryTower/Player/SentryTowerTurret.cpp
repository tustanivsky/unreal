// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerTurret.h"

#include "SentryTowerPlayerController.h"
#include "SentryTowerProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"

ASentryTowerTurret::ASentryTowerTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));

	RootComponent = TurretBase;

	TurretWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretWeapon"));
	TurretWeapon->SetupAttachment(RootComponent);

	ProjectileSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSocket"));
	ProjectileSocket->SetupAttachment(TurretWeapon);
}

void ASentryTowerTurret::RotateTurret(const FVector& Target)
{
	FVector Direction = Target - GetActorLocation();
	Direction = Direction.GetSafeNormal();

	FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(Direction);
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -40.0f, 0.0f);

	TurretWeapon->SetWorldRotation(NewRotation);
}

void ASentryTowerTurret::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerController = Cast<ASentryTowerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("ASentryTowerTurret: Failed to get player controller!"));
		return;
	}

	PlayerController->OnShoot.AddDynamic(this, &ASentryTowerTurret::Shoot);
}

void ASentryTowerTurret::Shoot(AActor* TargetActor, const FVector& TargetLocation)
{
	FVector SpawnLocation = ProjectileSocket->GetComponentLocation();
	FRotator SpawnRotation = TurretWeapon->GetComponentRotation();

	auto Projectile = 
		Cast<ASentryTowerProjectile>(GetWorld()->SpawnActor(ProjectileType, &SpawnLocation, &SpawnRotation));
	
	Projectile->TargetToFollow = TargetActor;
	Projectile->TargetStationary = TargetLocation;

	Projectile->Init();

	UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);
}

void ASentryTowerTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

