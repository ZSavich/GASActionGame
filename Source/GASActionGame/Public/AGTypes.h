// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.generated.h"

class UAGCharacterAnimDataAsset;
class UGameplayAbility;
class UGameplayEffect;


USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TObjectPtr<UAGCharacterAnimDataAsset> CharacterAnimationDataAsset;
};

USTRUCT(BlueprintType)
struct FCharacterAnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TObjectPtr<UBlendSpace> MovementBlendSpace;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TObjectPtr<UAnimSequence> IdleAnimation;
};

UENUM()
enum EFoot
{
	EF_Left		UMETA(DisplayName = "Left"),
	EF_Right	UMETA(DisplayName = "Right")
};
