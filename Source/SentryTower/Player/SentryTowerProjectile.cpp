// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerProjectile.h"
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

void ASentryTowerProjectile::Init(const FVector& TargetLocation)
{
	Target = TargetLocation;
	Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	HasTarget = true;
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

	auto EnemyTarget = Cast<ASentryTowerEnemyBase>(OtherActor);
	if(EnemyTarget != nullptr)
	{
		UGameplayStatics::ApplyDamage(EnemyTarget, Damage, PlayerController, this, nullptr);
	}

	Destroy();
}

// Called when the game starts or when spawned
void ASentryTowerProjectile::BeginPlay()
{
	Super::BeginPlay();

	Body->OnComponentBeginOverlap.AddDynamic(this, &ASentryTowerProjectile::OnBeginOverlap);
}

// Called every frame
void ASentryTowerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasTarget)
	{
		FVector NewLocation = Direction * 2000.0f * DeltaTime + GetActorLocation();
		// FVector NewLocation = FMath::VInterpTo(GetActorLocation(), Target, DeltaTime, 10.0f);
		SetActorLocation(NewLocation);
	}
}

