// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbilityBase.h"
#include "AGGameplayAbility_Jump.generated.h"

UCLASS()
class GASACTIONGAME_API UAGGameplayAbility_Jump : public UAGGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UAGGameplayAbility_Jump();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Jump")
	FGameplayTag WallRunStateTag;

	UPROPERTY(EditDefaultsOnly, Category = "Jump")
	float OffWallJumpStrength;

};
