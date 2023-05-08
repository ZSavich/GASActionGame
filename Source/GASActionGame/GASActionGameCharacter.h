// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AGTypes.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GASActionGameCharacter.generated.h"

class UAGFootstepsComponent;
class UAGCharacterDataAsset;
class UGameplayAbility;
class UAGAbilitySystemComponent;
class UAGAttributeSetBase;
class UGameplayEffect;

UCLASS(config=Game)
class AGASActionGameCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	// Ability System Component used by this character
	UPROPERTY(EditDefaultsOnly, Transient)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Transient)
	TObjectPtr<UAGFootstepsComponent> FootstepsComponent;

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

public:
	AGASActionGameCharacter();

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
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;
	
	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_Jump(const FInputActionValue& Value);

	bool ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext) const;
	void GiveAbilities();
	void ApplyStartupEffects();
	
	void InitFromCharacterData(const FCharacterData& InCharacterData, const bool bFromReplication = false);
};

