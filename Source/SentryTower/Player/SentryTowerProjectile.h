// Copyright (c) 2024 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SentryTowerProjectile.generated.h"

class USphereComponent;

UCLASS()
class SENTRYTOWER_API ASentryTowerProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASentryTowerProjectile();

	void Init(AActor* TargetActor, const FVector& TargetLocation);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetDestroyed(AActor* DestroyedEnemy);

	UFUNCTION(BlueprintImplementableEvent)
	void ApplySpecialEffect(AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Body;

	/** Effect spawned on projectile hits the target */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ParticleEffect;

	/** Damage dealt to targets being hit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 10.0f;

	/** Radius around the projectile hit point within which damage is dealt to other enemies (for AOE projectiles) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius = 0.0f;

	/** Projectile flying speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed = 3000.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	AActor* TargetToFollow;
	FVector TargetStationary;
	FVector Direction;
};
