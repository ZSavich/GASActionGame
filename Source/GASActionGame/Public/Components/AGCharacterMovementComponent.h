// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AGCharacterMovementComponent.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;

UCLASS()
class GASACTIONGAME_API UAGCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> TraversalAbilities;

public:
	bool TryTraversal(UAbilitySystemComponent* AbilitySystemComponent);
};
