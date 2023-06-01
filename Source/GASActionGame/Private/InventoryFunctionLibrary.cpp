// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryFunctionLibrary.h"
#include "AGTypes.h"

UItemStaticData* UInventoryFunctionLibrary::GetItemStaticData(const TSubclassOf<UItemStaticData> ItemDataClass)
{
	if (IsValid(ItemDataClass))
	{
		return ItemDataClass->GetDefaultObject<UItemStaticData>();
	}
	return nullptr;
}
