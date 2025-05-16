// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SentryTowerGameInstance.generated.h"

UCLASS()
class SENTRYTOWER_API USentryTowerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};
