// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.generated.h"

class UGameplayAbility;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly,Category = "Character")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
};