// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SentryTowerPawn.h"
#include "SentryTowerTurret.h"
#include "SentryTower/Enemy/SentryTowerEnemyBase.h"

void ASentryTowerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ASentryTowerPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CursorTrace();
}

void ASentryTowerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(ShootClickAction, ETriggerEvent::Started, this, &ASentryTowerPlayerController::OnShootStarted);

		// Setup touch input events
		EnhancedInputComponent->BindAction(ShootTouchAction, ETriggerEvent::Started, this, &ASentryTowerPlayerController::OnShootStarted);

		// Setup keyboard events
		EnhancedInputComponent->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &ASentryTowerPlayerController::OnOpenMenuStarted);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASentryTowerPlayerController: Failed to find an Enhanced Input Component!"));
	}
}

void ASentryTowerPlayerController::OnShootStarted()
{
	UE_LOG(LogTemp, Log, TEXT("ASentryTowerPlayerController: Shoot!"));

	if (AllowShooting)
	{
		FHitResult CursorHit;
		GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

		OnShoot.Broadcast(Cast<ASentryTowerEnemyBase>(CursorHit.GetActor()), CursorHit.Location);
	}
}

void ASentryTowerPlayerController::OnOpenMenuStarted()
{
	UE_LOG(LogTemp, Log, TEXT("ASentryTowerPlayerController: Open menu!"));

	OnOpenMenu.Broadcast();
}

void ASentryTowerPlayerController::CursorTrace()
{
	auto TowerPawn = Cast<ASentryTowerPawn>(GetPawn());
	if (!TowerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASentryTowerPlayerController: Can't get tower pawn!"));
		return;
	}

	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// check if tower isn't aiming at itself
	AllowShooting = !Cast<ASentryTowerPawn>(CursorHit.GetActor()) && !Cast<ASentryTowerTurret>(CursorHit.GetActor());

	TowerPawn->RotateTurret(CursorHit.Location);
}
