// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbilityBase.h"
#include "AGGameplayAbility_WallRun.generated.h"

class UAbilityTask_TickWallRun;

UCLASS()
class GASACTIONGAME_API UAGGameplayAbility_WallRun : public UAGGameplayAbilityBase
{
	GENERATED_BODY()

	UAGGameplayAbility_WallRun();

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnWallSideDetermined(bool bLeftSide);

	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_TickWallRun> WallRunTickTask;

	UPROPERTY(EditDefaultsOnly, Category = "WallRun")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "WallRun")
	TSubclassOf<UGameplayEffect> WallRunLeftSideEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "WallRun")
	TSubclassOf<UGameplayEffect> WallRunRightSideEffectClass;
	
};
