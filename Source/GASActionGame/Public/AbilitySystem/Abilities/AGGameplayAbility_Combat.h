// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbility_Inventory.h"
#include "AGGameplayAbility_Combat.generated.h"

UCLASS()
class GASACTIONGAME_API UAGGameplayAbility_Combat : public UAGGameplayAbility_Inventory
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FGameplayEffectSpecHandle GetWeaponEffectSpec(const FHitResult& HitResult) const;

	UFUNCTION(BlueprintPure)
	bool GetWeaponToFocusTraceResult(const float TraceDistance, const ETraceTypeQuery TraceType, FHitResult& OutHitResult) const;
};
