// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AGGameplayAbility_Jump.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GASActionGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UAGGameplayAbility_Jump::UAGGameplayAbility_Jump()
{
	// The ability is executed on both on the client and the server.
	// The client predicts the result and applies it locally, while the server verifies
	// the prediction and corrects any discrepancies.
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
	// Don't need unique variables of parameters for each activation.
	// Only need to be activated by a single actor at a time.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	OffWallJumpStrength = 100.f;
}

bool UAGGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (ACharacter* AvatarCharacter = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		const bool bMovementAllowsJump = AvatarCharacter->GetCharacterMovement()->IsJumpAllowed();
		const UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AvatarCharacter);
		const bool bIsWallRunning = AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(WallRunStateTag);
		
		return AvatarCharacter->CanJump() || (bMovementAllowsJump && bIsWallRunning);
	}
	
	return false;
}

void UAGGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Check if the ability is being activated by the server or by a prediction client
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		// Attempt to commit the ability to ensure it can be activated (calls CommitCost and CommitCooldown)
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			return;
		}
		
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

		// Get the actor that activated the ability
		if (AGASActionGameCharacter* AvatarCharacter = CastChecked<AGASActionGameCharacter>(ActorInfo->AvatarActor.Get()))
		{
			UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AvatarCharacter);
			if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(WallRunStateTag))
			{
				const FGameplayTagContainer WallRunTags(WallRunStateTag);
				AbilitySystemComponent->CancelAbilities(&WallRunTags);
				const FVector JumpOffVector = AvatarCharacter->GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal() + FVector::UpVector;
				AvatarCharacter->LaunchCharacter(JumpOffVector * OffWallJumpStrength, true, true);
			}
			else
			{
				// Call the Jump on the Character
				AvatarCharacter->Jump();
				
			}
		}
	}
}
