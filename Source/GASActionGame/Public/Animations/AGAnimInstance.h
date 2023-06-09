// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AGAnimInstance.generated.h"

class UItemStaticData;
class UAGCharacterAnimDataAsset;

UCLASS()
class GASACTIONGAME_API UAGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAGCharacterAnimDataAsset> DefaultCharacterAnimationData;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation", Meta = (BlueprintThreadSafe))
	UItemStaticData* GetEquippedItemStaticData();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation", Meta = (BlueprintThreadSafe))
	UBlendSpace* GetMovementBlendSpace();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation", Meta = (BlueprintThreadSafe))
	UAnimSequence* GetIdleAnimation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation", Meta = (BlueprintThreadSafe))
	UBlendSpace* GetCrouchMovementBlendSpace();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animation", Meta = (BlueprintThreadSafe))
	UAnimSequence* GetCrouchIdleAnimation();
};
