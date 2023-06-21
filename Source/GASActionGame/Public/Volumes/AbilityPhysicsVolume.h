// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameFramework/PhysicsVolume.h"
#include "GameplayEffectTypes.h"
#include "AbilityPhysicsVolume.generated.h"

class UGameplayAbility;
class UGameplayEffect;

USTRUCT()
struct FAbilityVolumeEnteredActorInfo
{
	GENERATED_BODY()

public:
	TArray<FGameplayAbilitySpecHandle> AppliedAbilities;
	TArray<FActiveGameplayEffectHandle> AppliedEffects;
};

UCLASS()
class GASACTIONGAME_API AAbilityPhysicsVolume : public APhysicsVolume
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDrawDebug;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToApply;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OnExitEffectsToApply;

	UPROPERTY(EditAnywhere, Category = "Events")
	TArray<FGameplayTag> GameplayEventsToSendOnEnter;

	UPROPERTY(EditAnywhere, Category = "Events")
	TArray<FGameplayTag> GameplayEventsToSendOnExit;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> OngoingAbilitiesToGive;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> PermanentAbilitiesToGive;

	UPROPERTY(Transient)
	TMap<AActor*, FAbilityVolumeEnteredActorInfo> EnteredActorsInfoMap;

public:
	AAbilityPhysicsVolume();

	virtual void Tick(float DeltaSeconds) override;
	virtual void ActorEnteredVolume(AActor* Other) override;
	virtual void ActorLeavingVolume(AActor* Other) override;
};
