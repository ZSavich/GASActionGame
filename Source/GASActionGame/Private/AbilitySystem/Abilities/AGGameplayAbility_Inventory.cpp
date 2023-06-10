// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AGGameplayAbility_Inventory.h"

#include "AGTypes.h"
#include "Components/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/WeaponItemActor.h"

void UAGGameplayAbility_Inventory::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	InventoryComponent = ActorInfo->OwnerActor->FindComponentByClass<UInventoryComponent>();
	check(InventoryComponent);
}

UInventoryComponent* UAGGameplayAbility_Inventory::GetInventoryComponent() const
{
	return InventoryComponent;
}

UInventoryItemInstance* UAGGameplayAbility_Inventory::GetEquippedItemInstance() const
{
	return InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;
}

UItemStaticData* UAGGameplayAbility_Inventory::GetEquippedStaticData() const
{
	const UInventoryItemInstance* EquippedItem = GetEquippedItemInstance();
	return EquippedItem ? EquippedItem->GetItemStaticData() : nullptr;
}

UWeaponStaticData* UAGGameplayAbility_Inventory::GetEquippedWeaponStaticData() const
{
	return Cast<UWeaponStaticData>(GetEquippedStaticData());
}

AItemActor* UAGGameplayAbility_Inventory::GetEquippedItemActor() const
{
	const UInventoryItemInstance* EquippedItem = GetEquippedItemInstance();
	return EquippedItem ? EquippedItem->GetItemActor() : nullptr;
}

AWeaponItemActor* UAGGameplayAbility_Inventory::GetEquippedWeaponActor() const
{
	return Cast<AWeaponItemActor>(GetEquippedItemActor());
}
