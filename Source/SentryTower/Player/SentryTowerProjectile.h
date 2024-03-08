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

	void Init(const FVector& TargetLocation);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void ApplySpecialEffect(AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ParticleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector Target;
	FVector Direction;
	bool HasTarget = false;
};
