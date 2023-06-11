// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AGGameplayAbility_Combat.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AGTypes.h"

FGameplayEffectSpecHandle UAGGameplayAbility_Combat::GetWeaponEffectSpec(const FHitResult& HitResult) const
{
	const UWeaponStaticData* WeaponStaticData = GetEquippedWeaponStaticData();
	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystemComponent && WeaponStaticData && WeaponStaticData->DamageEffect && WeaponStaticData->BaseDamage > 0.f)
	{
		const FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		if (EffectContext.IsValid())
		{
			const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(WeaponStaticData->DamageEffect, 1.f, EffectContext);
			if (SpecHandle.IsValid())
			{
				const FGameplayTag HealthDataTag = FGameplayTag::RequestGameplayTag(TEXT("Attribute.Health"));
				return UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, HealthDataTag, -WeaponStaticData->BaseDamage);
			}
		}
	}
	return FGameplayEffectSpecHandle();
}
