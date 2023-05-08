// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AGGameplayAbility_Jump.h"

#include "AbilitySystemComponent.h"
#include "GASActionGameCharacter.h"

UAGGameplayAbility_Jump::UAGGameplayAbility_Jump()
{
	// The ability is executed on both on the client and the server.
	// The client predicts the result and applies it locally, while the server verifies
	// the prediction and corrects any discrepancies.
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
	// Don't need unique variables of parameters for each activation.
	// Only need to be activated by a single actor at a time.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

bool UAGGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                 const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (const ACharacter* AvatarCharacter = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		return AvatarCharacter->CanJump();
	}
	return false;
}

void UAGGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Check if the ability is being activated by the server or by a prediction client
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		// Attempt to commit the ability to ensure it can be activated (calls CommitCost and CommitCooldown)
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			return;
		}

		// Get the actor that activated the ability
		if (AGASActionGameCharacter* AvatarCharacter = CastChecked<AGASActionGameCharacter>(ActorInfo->AvatarActor.Get()))
		{
			// Call the Jump on the Character
			AvatarCharacter->Jump();

			// Get the AbilitySystemComponent of the actor that activated the ability
			if (UAbilitySystemComponent* AbilityComponent = AvatarCharacter->GetAbilitySystemComponent())
			{
				// Provides information about the execution of the Gameplay Effect. This information can include things
				// like the owning actor, the Hit Result, and the source of the effect.
				const FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();

				// Creates an Instance of the GameplayEffect.
				// If the SpecHandle is valid, the owning character's ability system can apply the effect to the owning character
				const FGameplayEffectSpecHandle SpecHandle = AbilityComponent->MakeOutgoingSpec(JumpEffect, 1, EffectContext);

				if (SpecHandle.IsValid())
				{
					// Applies the Gameplay Effect to the owning character.
					// If the effect is successfully applied, it returns FActiveGameplayEffectHandle
					// which can be used to modify or remove the effect later.
					const FActiveGameplayEffectHandle ActiveGEHandle = AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					if (!ActiveGEHandle.IsValid() || !ActiveGEHandle.WasSuccessfullyApplied())
					{
						UE_LOG(LogTemp, Error, TEXT("Jump Ability Failed to Activate"));
					}
				}
			}
		}
	}
}
