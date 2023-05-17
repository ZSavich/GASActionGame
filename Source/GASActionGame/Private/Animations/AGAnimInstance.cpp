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
			if (UBlendSpace* BlendSpace = CharacterData->CharacterAnimationData.MovementBlendSpace)
			{
				return BlendSpace;
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
			if (UAnimSequence* Animation = CharacterData->CharacterAnimationData.IdleAnimation)
			{
				return Animation;
			}
		}
	}
	return DefaultCharacterAnimationData ? DefaultCharacterAnimationData->CharacterAnimationData.IdleAnimation : nullptr;
}

UBlendSpace* UAGAnimInstance::GetCrouchMovementBlendSpace()
{
	if (const AGASActionGameCharacter* ActionCharacter = Cast<AGASActionGameCharacter>(GetOwningActor()))
	{
		if (const UAGCharacterAnimDataAsset* CharacterData = ActionCharacter->GetCharacterData().CharacterAnimationDataAsset)
		{
			if (UBlendSpace* BlendSpace = CharacterData->CharacterAnimationData.CrouchBlendSpace)
			{
				return BlendSpace;
			}
		}
	}
	return DefaultCharacterAnimationData ? DefaultCharacterAnimationData->CharacterAnimationData.CrouchBlendSpace : nullptr;
}

UAnimSequence* UAGAnimInstance::GetCrouchIdleAnimation()
{
	if (const AGASActionGameCharacter* ActionCharacter = Cast<AGASActionGameCharacter>(GetOwningActor()))
	{
		if (const UAGCharacterAnimDataAsset* CharacterData = ActionCharacter->GetCharacterData().CharacterAnimationDataAsset)
		{
			if (UAnimSequence* Animation = CharacterData->CharacterAnimationData.CrouchIdleAnimation)
			{
				return Animation;
			}
		}
	}
	return DefaultCharacterAnimationData ? DefaultCharacterAnimationData->CharacterAnimationData.CrouchIdleAnimation : nullptr;
}
