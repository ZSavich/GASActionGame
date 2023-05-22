// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AGGameplayAbilityBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "GASActionGameCharacter.h"

void UAGGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get the AbilitySystemComponent of the actor that activated the ability
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		// Provides information about the execution of the Gameplay Effect. This information can include things
		// like the owning actor, the Hit Result, and the source of the effect.
		const FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();

		// Apply Gameplay Effects that should be applied on start and will not remove on the end
		for (const auto& GameplayEffect : OngoingEffectsApplyOnStart)
		{
			if (!GameplayEffect.Get()) continue;

			// Creates an Instance of the GameplayEffect.
			// If the SpecHandle is valid, the owning character's ability system can apply the effect to the owning character
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
			if (SpecHandle.IsValid())
			{
				// Applies the Gameplay Effect to the owning character.
				// If the effect is successfully applied, it returns FActiveGameplayEffectHandle
				// which can be used to modify or remove the effect later.
				FActiveGameplayEffectHandle ActiveEffect = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if (!ActiveEffect.WasSuccessfullyApplied())
				{
					ABILITY_LOG(Error, TEXT("Failed to apply the %s gameplay effect."), *GetNameSafe(GameplayEffect));
				}
			}
		}

		if (IsInstantiated())
		{
			// Apply Gameplay Effect that should be applied on start and will removed on the end
			for (const auto& GameplayEffect : OngoingEffectsRemoveOnEnd)
			{
				if (!GameplayEffect.Get()) continue;

				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
				if (SpecHandle.IsValid())
				{
					FActiveGameplayEffectHandle ActiveEffect = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					if (!ActiveEffect.WasSuccessfullyApplied())
					{
						ABILITY_LOG(Error, TEXT("Failed to apply the %s gameplay effect."), *GetNameSafe(GameplayEffect));
					}
					else
					{
						RemoveOnEndEffectHandles.Add(ActiveEffect);
					}
				}
			}
		}
	}
}

void UAGGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsInstantiated())
	{
		if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
		{
			for (const auto& EffectToRemove : RemoveOnEndEffectHandles)
			{
				if (!EffectToRemove.IsValid()) continue;

				ASC->RemoveActiveGameplayEffect(EffectToRemove);
			}
			RemoveOnEndEffectHandles.Empty();
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AGASActionGameCharacter* UAGGameplayAbilityBase::GetActionGameCharacterFromActorInfo() const
{
	return CastChecked<AGASActionGameCharacter>(GetAvatarActorFromActorInfo());
}
