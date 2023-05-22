// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AGGameplayAbilityBase.h"
#include "AGGameplayAbility_Vault.generated.h"

class UAbilityTask_PlayMontageAndWait;

UCLASS()
class GASACTIONGAME_API UAGGameplayAbility_Vault : public UAGGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Vault")
	TObjectPtr<UAnimMontage> VaultMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Vault")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Vault")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannelsToIgnore;
	
	UPROPERTY(EditDefaultsOnly, Category = "Vault|HorizontalTrace")
	float HorizontalTraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Vault|HorizontalTrace")
	float HorizontalTraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Vault|HorizontalTrace")
	float HorizontalTraceCount;

	UPROPERTY(EditDefaultsOnly, Category = "Vault|HorizontalTrace")
	float HorizontalTraceStep;

	UPROPERTY(EditDefaultsOnly, Category = "Vault|VerticalTrace")
	float VerticalTraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Vault|VerticalTrace")
	float VerticalTraceStep;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
	
	FVector JumpToLocation;
	FVector JumpOverLocation;

public:
	UAGGameplayAbility_Vault();
	
protected:
	virtual bool CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
};
