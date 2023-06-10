// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/AGAnimInstance.h"
#include "GASActionGameCharacter.h"
#include "Components/InventoryComponent.h"
#include "DataAssets/AGCharacterAnimDataAsset.h"
#include "Inventory/InventoryItemInstance.h"

UItemStaticData* UAGAnimInstance::GetEquippedItemStaticData()
{
	if (const AGASActionGameCharacter* ActionCharacter = Cast<AGASActionGameCharacter>(GetOwningActor()))
	{
		if (const UInventoryComponent* InventoryComponent = ActionCharacter->GetInventoryComponent())
		{
			if (const UInventoryItemInstance* EquippedItem = InventoryComponent->GetEquippedItem())
			{
				return EquippedItem->GetItemStaticData();
			}
		}
	}
	return nullptr;
}

UBlendSpace* UAGAnimInstance::GetMovementBlendSpace()
{
	if (const UItemStaticData* ItemStaticData = GetEquippedItemStaticData())
	{
		if (ItemStaticData->GetCharacterAnimationData().MovementBlendSpace)
		{
			return ItemStaticData->GetCharacterAnimationData().MovementBlendSpace;
		}
	}
	
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
	if (const UItemStaticData* ItemStaticData = GetEquippedItemStaticData())
	{
		if (ItemStaticData->GetCharacterAnimationData().IdleAnimation)
		{
			return ItemStaticData->GetCharacterAnimationData().IdleAnimation;
		}
	}
	
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
	if (const UItemStaticData* ItemStaticData = GetEquippedItemStaticData())
	{
		if (ItemStaticData->GetCharacterAnimationData().CrouchBlendSpace)
		{
			return ItemStaticData->GetCharacterAnimationData().CrouchBlendSpace;
		}
	}
	
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
	if (const UItemStaticData* ItemStaticData = GetEquippedItemStaticData())
	{
		if (ItemStaticData->GetCharacterAnimationData().CrouchIdleAnimation)
		{
			return ItemStaticData->GetCharacterAnimationData().CrouchIdleAnimation;
		}
	}
	
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
