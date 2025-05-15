// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerEnemyBase.h"

#include "AIController.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SentryTower/Player/SentryTowerPawn.h"
#include "SentryTower/UI/SentryTowerEnemyHealthbar.h"
#include "Sound/SoundBase.h"

ASentryTowerEnemyBase::ASentryTowerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	Body->SetCollisionObjectType(ECC_WorldDynamic);
	Body->SetCollisionResponseToAllChannels(ECR_Block);

	RootComponent = Body;

	Healthbar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Healthbar"));
	Healthbar->SetWidgetClass(HealthbarWidgetClass);
	Healthbar->SetupAttachment(RootComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->bConstrainToPlane = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASentryTowerEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	Body->OnComponentBeginOverlap.AddDynamic(this, &ASentryTowerEnemyBase::OnBeginOverlap);

	Health = MaxHealth;

	MoveTowardsTarget();
}

float ASentryTowerEnemyBase::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= Damage;

	if (Health <= 0.0f)
	{
		OnEnemyDies.Broadcast(ExpBonus);
		UGameplayStatics::SpawnEmitterAtLocation(this, ParticleEffect, GetActorLocation());
		UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);
		Destroy();
	}

	auto HealthbarWidget = Cast<USentryTowerEnemyHealthbar>(Healthbar->GetWidget());
	if (HealthbarWidget != nullptr)
	{
		HealthbarWidget->SetHealthValue(Health, MaxHealth);
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ASentryTowerEnemyBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Target = Cast<ASentryTowerPawn>(OtherActor);
	if (!Target)
	{
		return;
	}

	if (Cast<USphereComponent>(OtherComp))
	{
		// Got into tower's shooting range
		return;
	}

	UGameplayStatics::ApplyDamage(Target, EnemyDamage, GetController(), this, nullptr);
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleEffect, GetActorLocation());
	UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);
	Destroy();
}

void ASentryTowerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTowardsTarget();
}

void ASentryTowerEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASentryTowerEnemyBase::MoveTowardsTarget()
{
	auto TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!TargetPawn)
	{
		return;
	}

	auto AiController = Cast<AAIController>(GetController());
	if (!AiController)
	{
		return;
	}

	FVector TargetLocation = TargetPawn->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();

	AiController->MoveToLocation(FVector(TargetLocation.X - CurrentLocation.X, TargetLocation.Y - CurrentLocation.Y, 0.0f));
}

void ASentryTowerEnemyBase::RotateTowardsTarget()
{
	auto TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (TargetPawn == nullptr)
	{
		return;
	}

	FVector TargetLocation = TargetPawn->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	SetActorRotation(NewRotation);
}

