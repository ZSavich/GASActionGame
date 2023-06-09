// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.generated.h"

class UNiagaraSystem;
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

UENUM()
enum class EMovementDirectionType : uint8
{
	EMDT_None				UMETA(DisplayName = "None"),
	EMDT_OrientToMovement	UMETA(DisplayName = "OrientToMovement"),
	EMDT_Strafe				UMETA(DisplayName = "Strafe")
};

UCLASS(BlueprintType, Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffects;
	
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

UCLASS(BlueprintType, Blueprintable)
class UWeaponStaticData: public UItemStaticData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MuzzleSocketName = "Muzzle_Socket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> AttackSound;
};

UCLASS(BlueprintType, Blueprintable)
class UProjectileStaticData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GravityMultiplayer = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InitialSpeed = 3000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxSpeed = 3000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> RadialDamageQueryTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ETraceTypeQuery> RadialDamageTraceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> ImpactSFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> ImpactVFX;
	
};