// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AGGameplayAbility_Inventory.generated.h"

class AItemActor;
class AWeaponItemActor;
class UWeaponStaticData;
class UItemStaticData;
class UInventoryItemInstance;
class UInventoryComponent;

UCLASS()
class GASACTIONGAME_API UAGGameplayAbility_Inventory : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TObjectPtr<UInventoryComponent> InventoryComponent;

public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintPure)
	UInventoryItemInstance* GetEquippedItemInstance() const;

	UFUNCTION(BlueprintPure)
	UItemStaticData* GetEquippedStaticData() const;

	UFUNCTION(BlueprintPure)
	UWeaponStaticData* GetEquippedWeaponStaticData() const;

	UFUNCTION(BlueprintPure)
	AItemActor* GetEquippedItemActor() const;
	
	UFUNCTION(BlueprintPure)
	AWeaponItemActor* GetEquippedWeaponActor() const;

};
