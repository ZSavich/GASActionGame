// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_TickWallRun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWallRunWallSideDeterminedDelegate, bool, bIsLeftSide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWallRunFinishedDelegate);

UCLASS()
class GASACTIONGAME_API UAbilityTask_TickWallRun : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnWallRunWallSideDeterminedDelegate OnWallSideDetermined;

	UPROPERTY(BlueprintAssignable)
	FOnWallRunFinishedDelegate OnWallSideFinished;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> CharacterOwner;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	
public:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	virtual void TickTask(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAbilityTask_TickWallRun* CreateWallRunTask(UGameplayAbility* OwningAbility, ACharacter* InCharacterOwner, UCharacterMovementComponent* InCharacterMovement, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes);

	bool FindRunnableWall(FHitResult& OutHitResult);
	bool IsWallOnTheLeft(const FHitResult& InWallHitResult);
	
};
