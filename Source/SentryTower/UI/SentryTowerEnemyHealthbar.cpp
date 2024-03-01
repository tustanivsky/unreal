// Copyright (c) 2024 Sentry. All Rights Reserved.

#include "SentryTowerEnemyHealthbar.h"

#include "Components/ProgressBar.h"

void USentryTowerEnemyHealthbar::SetHealthValue(float Health, float MaxHealth)
{
	HeathProgreesBar->SetPercent(Health / MaxHealth);
}

void USentryTowerEnemyHealthbar::NativeConstruct()
{
	Super::NativeConstruct();

	HeathProgreesBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Progressbar_Health")));
}
