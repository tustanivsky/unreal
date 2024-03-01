// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SentryTowerEnemyHealthbar.generated.h"

class UProgressBar;

UCLASS()
class SENTRYTOWER_API USentryTowerEnemyHealthbar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthValue(float Health, float MaxHealth);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	UProgressBar* HeathProgreesBar;
};
