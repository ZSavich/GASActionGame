// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AGCharacterMovementComponent.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;

UCLASS()
class GASACTIONGAME_API UAGCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> TraversalAbilities;

	EMovementDirectionType MovementDirectionType;

public:
	virtual void BeginPlay() override;

	bool TryTraversal(UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintPure)
	EMovementDirectionType GetMovementDirectionType() const;

	UFUNCTION(BlueprintCallable)
	void SetMovementDirectionType(const EMovementDirectionType InMovementDirectionType);

protected:
	UFUNCTION()
	void OnEnforcedStrafeTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	void HandleMovementDirection();
};
