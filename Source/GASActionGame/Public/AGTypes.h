// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.generated.h"

class AItemActor;
class UAGCharacterAnimDataAsset;
class UGameplayAbility;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TObjectPtr<UAGCharacterAnimDataAsset> CharacterAnimationDataAsset;
};

USTRUCT(BlueprintType)
struct FCharacterAnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Character|Stand")
	TObjectPtr<UBlendSpace> MovementBlendSpace;

	UPROPERTY(EditDefaultsOnly, Category = "Character|Stand")
	TObjectPtr<UAnimSequence> IdleAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Character|Crouch")
	TObjectPtr<UBlendSpace> CrouchBlendSpace;

	UPROPERTY(EditDefaultsOnly, Category = "Character|Stand")
	TObjectPtr<UAnimSequence> CrouchIdleAnimation;
};

UENUM()
enum class EFoot : uint8
{
	EF_Left		UMETA(DisplayName = "Left"),
	EF_Right	UMETA(DisplayName = "Right")
};

UENUM()
enum class EItemState : uint8
{
	EIS_None		UMETA(DisplayName = "None"),
	EIS_Equipped	UMETA(DisplayName = "Equipped"),
	EIS_Dropped		UMETA(DisplayName = "Dropped")
};

UCLASS(BlueprintType, Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AItemActor> ItemActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachmentSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeEquipped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCharacterAnimData CharacterAnimationData;

public:
	FORCEINLINE const FName& GetItemName() const { return ItemName; }
	FORCEINLINE const FName& GetAttachmentSocketName() const { return AttachmentSocketName; }
	FORCEINLINE const TSubclassOf<AItemActor>& GetItemActorClass() const { return ItemActorClass; }
	FORCEINLINE bool CanBeEquipped() const { return bCanBeEquipped; }
	FORCEINLINE const FCharacterAnimData& GetCharacterAnimationData() const { return CharacterAnimationData; }
};