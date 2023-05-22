// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AGCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"

static TAutoConsoleVariable<bool> CVarShowTraversal(
	TEXT("AG.ShowDebugTraversal"),
	false,
	TEXT("Show debug information about motion warping movements."),
	ECVF_Cheat);

bool UAGCharacterMovementComponent::TryTraversal(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		for (const TSubclassOf<UGameplayAbility>& Ability : TraversalAbilities)
		{
			if (AbilitySystemComponent->TryActivateAbilityByClass(Ability))
			{
				const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(Ability);
				if (AbilitySpec && AbilitySpec->IsActive())
				{
					return true;
				}
			}
		}
	}
	return false;
}
