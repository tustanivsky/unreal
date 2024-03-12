// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SentryTowerPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShootDelegate, AActor*, TargetActor, const FVector&, TargetLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenMenuDelegate);

/**
 * Player controller
 */
UCLASS()
class SENTRYTOWER_API ASentryTowerPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

	/** Input handlers. */
	void OnShootStarted();
	void OnOpenMenuStarted();

	void CursorTrace();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Tower shoot input action (click) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootClickAction;

	/** Tower shoot input action (touch) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootTouchAction;

	/** Open game menu input action (touch) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* OpenMenuAction;

	UPROPERTY(BlueprintAssignable)
	FOnShootDelegate OnShoot;

	UPROPERTY(BlueprintAssignable)
	FOnOpenMenuDelegate OnOpenMenu;

private:
	bool AllowShooting;
};
