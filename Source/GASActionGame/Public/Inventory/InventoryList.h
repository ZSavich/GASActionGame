// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryList.generated.h"

class UItemStaticData;
class UInventoryItemInstance;

USTRUCT(BlueprintType)
struct GASACTIONGAME_API FInventoryListItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UInventoryItemInstance> ItemInstance;
	
};

USTRUCT(BlueprintType)
struct GASACTIONGAME_API FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>(Items, DeltaParms, *this);
	}
	
protected:
	UPROPERTY()
	TArray<FInventoryListItem> Items;

	void AddItem(TSubclassOf<UItemStaticData> InItemDataClass);
	void RemoveItem(TSubclassOf<UItemStaticData> InItemDataClass);
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};