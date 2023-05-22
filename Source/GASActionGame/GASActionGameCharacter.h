// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AGTypes.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GASActionGameCharacter.generated.h"

class UAGMotionWarpingComponent;
class UAGFootstepsComponent;
class UAGCharacterDataAsset;
class UGameplayAbility;
class UAGAbilitySystemComponent;
class UAGAttributeSetBase;
class UGameplayEffect;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

UCLASS(config=Game)
class AGASActionGameCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

protected:
	// Ability System Component used by this character
	UPROPERTY(EditDefaultsOnly, Transient)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Transient)
	TObjectPtr<UAGFootstepsComponent> FootstepsComponent;

	UPROPERTY(EditDefaultsOnly, Transient)
	TObjectPtr<UAGMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TObjectPtr<UAGCharacterDataAsset> CharacterDataAsset;

	// Attribute set used by this character
	UPROPERTY(Transient)
	TObjectPtr<const UAGAttributeSetBase> AttributeSet;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CharacterData)
	FCharacterData CharacterData;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities|Jump")
	FGameplayTag JumpEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities|Jump")
	FGameplayTagContainer InAirTags;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities|Crouch")
	FGameplayTagContainer CrouchTags;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities|Crouch")
	TSubclassOf<UGameplayEffect> CrouchStateEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities|Sprint")
	FGameplayTagContainer SprintTags;

	FDelegateHandle OnMaxMovementSpeedChanged;

public:
	AGASActionGameCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;

	virtual void OnRep_PlayerState() override;
	
	UFUNCTION()
	void OnRep_CharacterData();

	void SetCharacterData(const FCharacterData& InCharacterData);
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE FCharacterData GetCharacterData() const { return CharacterData; }
	FORCEINLINE UAGFootstepsComponent* GetFootstepsComponent() const { return FootstepsComponent; }
	FORCEINLINE UAGMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	
	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_Jump(const FInputActionValue& Value);
	void Input_CrouchStart(const FInputActionValue& Value);
	void Input_CrouchEnd(const FInputActionValue& Value);
	void Input_SprintStart(const FInputActionValue& Value);
	void Input_SprintEnded(const FInputActionValue& Value);

	void HandleOnMaxMovementSpeedChanged(const FOnAttributeChangeData& MaxMovementSpeedAttribute);

	bool ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext) const;
	void GiveAbilities();
	void ApplyStartupEffects();
	
	void InitFromCharacterData(const FCharacterData& InCharacterData, const bool bFromReplication = false);
};

