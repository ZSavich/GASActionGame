// Fill out your copyright notice in the Description page of Project Settings.

#include "Volumes/AbilityPhysicsVolume.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAbilityPhysicsVolume::AAbilityPhysicsVolume()
{
	PrimaryActorTick.bCanEverTick = true;
	bDrawDebug = true;
}

void AAbilityPhysicsVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bDrawDebug)
	{
		DrawDebugBox(GetWorld(), GetActorLocation(), GetBounds().BoxExtent, FColor::Red, false, 0.f, 0, 5.f);
	}
}

void AAbilityPhysicsVolume::ActorEnteredVolume(AActor* Other)
{
	Super::ActorEnteredVolume(Other);

	if (!HasAuthority() || !Other)
	{
		return;
	}

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Other))
	{
		EnteredActorsInfoMap.Add(Other);
		
		for (TSubclassOf<UGameplayAbility> Ability : PermanentAbilitiesToGive)
		{
			if (Ability)
			{
				AbilitySystemComponent->GiveAbility(Ability);
			}
		}
		for (TSubclassOf<UGameplayAbility> Ability : OngoingAbilitiesToGive)
		{
			if (Ability)
			{
				FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(Ability);
				if (SpecHandle.IsValid())
				{
					EnteredActorsInfoMap.Find(Other)->AppliedAbilities.Add(SpecHandle);
				}
			}
		}
		for (TSubclassOf<UGameplayEffect> Effect : OngoingEffectsToApply)
		{
			if (Effect)
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
				EffectContext.AddInstigator(Other, Other);
				if (EffectContext.IsValid())
				{
					FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, EffectContext);
					if (SpecHandle.IsValid())
					{
						FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
						if (ActiveEffectHandle.WasSuccessfullyApplied())
						{
							EnteredActorsInfoMap.Find(Other)->AppliedEffects.Add(ActiveEffectHandle);
						}
					}
				}
			}
		}
		for (const FGameplayTag& EventTag : GameplayEventsToSendOnEnter)
		{
			FGameplayEventData EventData;
			EventData.EventTag = EventTag;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, EventTag, EventData);
		}
	}
}

void AAbilityPhysicsVolume::ActorLeavingVolume(AActor* Other)
{
	Super::ActorLeavingVolume(Other);

	if (!HasAuthority() || !Other)
	{
		return;
	}

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Other))
	{
		if (const FAbilityVolumeEnteredActorInfo* ActorInfo = EnteredActorsInfoMap.Find(Other))
		{
			for (const FGameplayAbilitySpecHandle& SpecHandle : ActorInfo->AppliedAbilities)
			{
				AbilitySystemComponent->ClearAbility(SpecHandle);
			}
			for (const FActiveGameplayEffectHandle& EffectHandle : ActorInfo->AppliedEffects)
			{
				AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
			}
			EnteredActorsInfoMap.Remove(Other);
		}

		for (TSubclassOf<UGameplayEffect> Effect : OnExitEffectsToApply)
		{
			if (Effect)
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
				EffectContext.AddInstigator(Other, Other);
				if (EffectContext.IsValid())
				{
					FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, EffectContext);
					if (SpecHandle.IsValid())
					{
						FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
			}
		}
		for (const FGameplayTag& EventTag : GameplayEventsToSendOnExit)
		{
			FGameplayEventData EventData;
			EventData.EventTag = EventTag;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, EventTag, EventData);
		}
	}
}
