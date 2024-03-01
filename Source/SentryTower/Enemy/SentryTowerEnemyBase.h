// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SentryTowerEnemyBase.generated.h"

class UWidgetComponent;
class USentryTowerEnemyHealthbar;
class UFloatingPawnMovement;

/**
 * Enemy pawn which attacks the tower
 */
UCLASS()
class SENTRYTOWER_API ASentryTowerEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASentryTowerEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> Healthbar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USentryTowerEnemyHealthbar> HealthbarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float EnemyDamage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Health;

private:
	void MoveTowardsTarget();
	void RotateTowardsTarget();
};
