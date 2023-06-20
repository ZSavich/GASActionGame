// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UProjectileStaticData;

UCLASS()
class GASACTIONGAME_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Replicated)
	TSubclassOf<UProjectileStaticData> ProjectileStaticDataClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
public:	
	AProjectile();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);
	
	void DebugDrawPath();
	
	const UProjectileStaticData* GetProjectileStaticData() const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
