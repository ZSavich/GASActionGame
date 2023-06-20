// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AGGameplayAbilityBase.generated.h"

class AGASActionGameCharacter;

UCLASS()
class GASACTIONGAME_API UAGGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayEffects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsRemoveOnEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayEffects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsApplyOnStart;

	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintPure)
	AGASActionGameCharacter* GetActionGameCharacterFromActorInfo() const;
};
