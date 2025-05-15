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
}

void ASentryTowerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup keyboard events
		EnhancedInputComponent->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &ASentryTowerPlayerController::OnOpenMenuStarted);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ASentryTowerPlayerController: Failed to find an Enhanced Input Component!"));
	}
}

void ASentryTowerPlayerController::OnOpenMenuStarted()
{
	UE_LOG(LogTemp, Log, TEXT("ASentryTowerPlayerController: Open menu!"));

	OnOpenMenu.Broadcast();
}
