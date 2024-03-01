// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerEnemyBase.h"

#include "AIController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SentryTower/Player/SentryTowerPawn.h"

// Sets default values
ASentryTowerEnemyBase::ASentryTowerEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	Body->SetCollisionObjectType(ECC_WorldDynamic);
	Body->SetCollisionResponseToAllChannels(ECR_Block);

	RootComponent = Body;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->bConstrainToPlane = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ASentryTowerEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	Body->OnComponentBeginOverlap.AddDynamic(this, &ASentryTowerEnemyBase::OnBeginOverlap);

	Health = MaxHealth;

	MoveTowardsTarget();
}

void ASentryTowerEnemyBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Target = Cast<ASentryTowerPawn>(OtherActor);
	if(Target != nullptr)
	{
		UGameplayStatics::ApplyDamage(Target, EnemyDamage, GetController(), this, nullptr);
		Destroy();
	}
}

float ASentryTowerEnemyBase::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= Damage;

	if(Health <= 0.0f)
	{
		Destroy();
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

// Called every frame
void ASentryTowerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTowardsTarget();
}

// Called to bind functionality to input
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

