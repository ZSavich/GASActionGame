// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/AGAnimInstance.h"
#include "GASActionGameCharacter.h"
#include "DataAssets/AGCharacterAnimDataAsset.h"

UBlendSpace* UAGAnimInstance::GetMovementBlendSpace()
{
	if (const AGASActionGameCharacter* ActionCharacter = Cast<AGASActionGameCharacter>(GetOwningActor()))
	{
		if (const UAGCharacterAnimDataAsset* CharacterData = ActionCharacter->GetCharacterData().CharacterAnimationDataAsset)
		{
			if (UBlendSpace* MovementBlendSpace = CharacterData->CharacterAnimationData.MovementBlendSpace)
			{
				return MovementBlendSpace;
			}
		}
	}
	return DefaultCharacterAnimationData ? DefaultCharacterAnimationData->CharacterAnimationData.MovementBlendSpace : nullptr;
}

UAnimSequence* UAGAnimInstance::GetIdleAnimation()
{
	if (const AGASActionGameCharacter* ActionCharacter = Cast<AGASActionGameCharacter>(GetOwningActor()))
	{
		if (const UAGCharacterAnimDataAsset* CharacterData = ActionCharacter->GetCharacterData().CharacterAnimationDataAsset)
		{
			if (UAnimSequence* IdleAnimation = CharacterData->CharacterAnimationData.IdleAnimation)
			{
				return IdleAnimation;
			}
		}
	}
	return DefaultCharacterAnimationData ? DefaultCharacterAnimationData->CharacterAnimationData.IdleAnimation : nullptr;
}
