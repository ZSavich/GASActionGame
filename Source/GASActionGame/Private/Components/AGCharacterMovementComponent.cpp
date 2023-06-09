// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AGCharacterMovementComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AGTypes.h"
#include "GameFramework/Character.h"

static TAutoConsoleVariable<bool> CVarShowTraversal(
	TEXT("AG.ShowDebugTraversal"),
	false,
	TEXT("Show debug information about motion warping movements."),
	ECVF_Cheat);

void UAGCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	HandleMovementDirection();

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		const FGameplayTag StrafeTag = FGameplayTag::RequestGameplayTag(TEXT("Movement.Enforced.Strafe"), EGameplayTagEventType::NewOrRemoved);
		AbilitySystemComponent->RegisterGameplayTagEvent(StrafeTag).AddUObject(this, &ThisClass::OnEnforcedStrafeTagChanged);
	}
}

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

EMovementDirectionType UAGCharacterMovementComponent::GetMovementDirectionType() const
{
	return MovementDirectionType;
}

void UAGCharacterMovementComponent::SetMovementDirectionType(const EMovementDirectionType InMovementDirectionType)
{
	MovementDirectionType = InMovementDirectionType;
	HandleMovementDirection();
}

void UAGCharacterMovementComponent::HandleMovementDirection()
{
	switch (MovementDirectionType)
	{
	case EMovementDirectionType::EMDT_Strafe:
		bUseControllerDesiredRotation = true;
		bOrientRotationToMovement = false;
		CharacterOwner->bUseControllerRotationYaw = true;
		break;
	default:
		bUseControllerDesiredRotation = false;
		bOrientRotationToMovement = true;
		CharacterOwner->bUseControllerRotationYaw = false;
		break;
	}
}

void UAGCharacterMovementComponent::OnEnforcedStrafeTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	if (NewCount)
	{
		SetMovementDirectionType(EMovementDirectionType::EMDT_Strafe);
	}
	else
	{
		SetMovementDirectionType(EMovementDirectionType::EMDT_OrientToMovement);
	}
}
