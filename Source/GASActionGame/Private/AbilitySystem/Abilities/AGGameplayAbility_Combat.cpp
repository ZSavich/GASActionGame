// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AGGameplayAbility_Combat.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AGTypes.h"
#include "GASActionGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Inventory/WeaponItemActor.h"
#include "Kismet/KismetSystemLibrary.h"

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

bool UAGGameplayAbility_Combat::GetWeaponToFocusTraceResult(const float TraceDistance, const ETraceTypeQuery TraceType, FHitResult& OutHitResult) const
{
	const AWeaponItemActor* WeaponItemActor = GetEquippedWeaponActor();
	const AGASActionGameCharacter* Character = GetActionGameCharacterFromActorInfo();
	if (WeaponItemActor && Character)
	{
		const FTransform CameraTransform = Character->GetFollowCamera()->GetComponentTransform();
		FVector FocusTraceEnd = CameraTransform.GetLocation() + CameraTransform.GetRotation().Vector() * TraceDistance;
		TArray<AActor*> ActorsToIgnore {GetAvatarActorFromActorInfo()};

		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(this, CameraTransform.GetLocation(), FocusTraceEnd, TraceType, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

		FVector MuzzleLocation = WeaponItemActor->GetMuzzleLocation();
		FVector WeaponTraceEnd = MuzzleLocation + (HitResult.Location - MuzzleLocation).GetSafeNormal() * TraceDistance;

		UKismetSystemLibrary::LineTraceSingle(this, MuzzleLocation, WeaponTraceEnd, TraceType, false, ActorsToIgnore, EDrawDebugTrace::None,	OutHitResult, true);
		return OutHitResult.bBlockingHit;
	}
	return false;
}
