// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SentryTowerEnemyBase.generated.h"

class UWidgetComponent;
class UFloatingPawnMovement;
class USoundBase;
class USentryTowerEnemyHealthbar;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDiesDelegate, int, ExpBonus);

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
	// Apply damage to this actor
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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

	/** Effect spawned right before enemy dies */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ParticleEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USentryTowerEnemyHealthbar> HealthbarWidgetClass;

	/** Damage that is dealt to tower whenever enemy reaches it */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EnemyDamage = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health;

	/** Experience granted to player when enemy is killed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ExpBonus = 10;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyDiesDelegate OnEnemyDies;

private:
	void MoveTowardsTarget();
	void RotateTowardsTarget();
};
