// Copyright (c) 2024 Sentry. All Rights Reserved.


#include "SentryTowerTurret.h"
#include "SentryTowerPlayerController.h"
#include "SentryTowerProjectile.h"

#include "Kismet/GameplayStatics.h"
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

	ProjectileSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSocket"));
	ProjectileSocket->SetupAttachment(TurretWeapon);
}

void ASentryTowerTurret::RotateTurret(const FVector& Target)
{
	TargetLocation = Target;

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

	auto PlayerController = Cast<ASentryTowerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("ASentryTowerTurret: Failed to get player controller!"));
		return;
	}

	PlayerController->OnShoot.AddDynamic(this, &ASentryTowerTurret::Shoot);
}

void ASentryTowerTurret::Shoot()
{
	FVector SpawnLocation = ProjectileSocket->GetComponentLocation();
	FRotator SpawnRotation = TurretWeapon->GetComponentRotation();

	auto Projectile = GetWorld()->SpawnActor(ProjectileType, &SpawnLocation, &SpawnRotation);

	Cast<ASentryTowerProjectile>(Projectile)->Init(TargetLocation);
}

// Called every frame
void ASentryTowerTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

