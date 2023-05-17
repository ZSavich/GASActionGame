// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AGGameplayAbility_Crouch.h"
#include "GameFramework/Character.h"

UAGGameplayAbility_Crouch::UAGGameplayAbility_Crouch()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UAGGameplayAbility_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		return Character->CanCrouch();
	}
	return false;
}

void UAGGameplayAbility_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			return;
		}
		
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

		if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor))
		{
			Character->Crouch();
		}
	}
}

void UAGGameplayAbility_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		Character->UnCrouch();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
