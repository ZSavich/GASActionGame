// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryList.h"
#include "AGTypes.h"
#include "Inventory/InventoryItemInstance.h"

void FInventoryList::AddItem(TSubclassOf<UItemStaticData> InItemDataClass)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UInventoryItemInstance>();
	Item.ItemInstance->Init(InItemDataClass);
	MarkItemDirty(Item);
}

void FInventoryList::RemoveItem(TSubclassOf<UItemStaticData> InItemDataClass)
{
	for (auto It = Items.CreateIterator() ; It; ++It)
	{
		FInventoryListItem& Item = *It;
		if (Item.ItemInstance && Item.ItemInstance->GetItemStaticData()->IsA(InItemDataClass))
		{
			It.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}
