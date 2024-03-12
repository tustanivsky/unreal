// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerProjectile.h"

#include "SentryTowerPawn.h"
#include "SentryTowerPlayerController.h"
#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"
#include "SentryTower/Enemy/SentryTowerEnemyBase.h"

// Sets default values
ASentryTowerProjectile::ASentryTowerProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));

	RootComponent = Body;
}

void ASentryTowerProjectile::Init(AActor* TargetActor, const FVector& TargetLocation)
{
	TargetToFollow = TargetActor;
	if (TargetToFollow != nullptr)
	{
		TargetToFollow->OnDestroyed.AddDynamic(this, &ASentryTowerProjectile::OnTargetDestroyed);
	}

	TargetStationary = TargetLocation;

	Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();

	SetActorTickEnabled(true);
}

void ASentryTowerProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	auto PlayerController = Cast<ASentryTowerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("ASentryTowerProjectile: Failed to get player controller!"));
		return;
	}

	auto TowerPawn = Cast<ASentryTowerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!TowerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASentryTowerProjectile: Can't get tower pawn!"));
		return;
	}

	float ResultingDamage = Damage + 0.1f * Damage * (TowerPawn->CurrentLevel - 1);

	if (Radius > 0.0f)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), ResultingDamage, GetActorLocation(), Radius, nullptr, { this, TowerPawn }, this);
	}
	else
	{
		auto EnemyTarget = Cast<ASentryTowerEnemyBase>(OtherActor);
		if(EnemyTarget != nullptr)
		{
			ApplySpecialEffect(OtherActor);
			UGameplayStatics::ApplyDamage(EnemyTarget, ResultingDamage, PlayerController, this, nullptr);
		}
	}

	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleEffect, GetActorLocation());

	Destroy();
}

void ASentryTowerProjectile::OnTargetDestroyed(AActor* DestroyedEnemy)
{
	Direction = (DestroyedEnemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	TargetToFollow = nullptr;
}

// Called when the game starts or when spawned
void ASentryTowerProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	Body->OnComponentBeginOverlap.AddDynamic(this, &ASentryTowerProjectile::OnBeginOverlap);
}

// Called every frame
void ASentryTowerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetToFollow != nullptr)
	{
		FVector EnemyDirection = (TargetToFollow->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = EnemyDirection * 3000.0f * DeltaTime + GetActorLocation();
		SetActorLocation(NewLocation);
	}
	else
	{
		FVector NewLocation = Direction * 3000.0f * DeltaTime + GetActorLocation();
		SetActorLocation(NewLocation);
	}
}

