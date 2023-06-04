// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemInstance.h"

#include "InventoryFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

void UInventoryItemInstance::OnEquipped()
{
}

void UInventoryItemInstance::OnUnequipped()
{
}

UItemStaticData* UInventoryItemInstance::GetItemStaticData() const
{
	return UInventoryFunctionLibrary::GetItemStaticData(ItemStaticDataClass);
}

void UInventoryItemInstance::OnRep_IsEquipped()
{
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsEquipped);
	DOREPLIFETIME(ThisClass, ItemStaticDataClass);
}