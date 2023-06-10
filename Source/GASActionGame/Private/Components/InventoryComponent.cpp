// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AGTypes.h"
#include "GameplayTagsManager.h"
#include "Inventory/InventoryItemInstance.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

TAutoConsoleVariable<bool> CVarShowDebugInventory(TEXT("AG.ShowDebugInventory"), false, TEXT("Print information about inventory in output log."), ECVF_Cheat);

FGameplayTag UInventoryComponent::EquipItemActorTag;
FGameplayTag UInventoryComponent::EquipNextTag;
FGameplayTag UInventoryComponent::DropItemTag;
FGameplayTag UInventoryComponent::UnequipTag;

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	static bool bHandleAddingTags = false;
	if (!bHandleAddingTags)
	{
		bHandleAddingTags = true;
		UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddUObject(this, &UInventoryComponent::AddInventoryTags);
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CVarShowDebugInventory.GetValueOnAnyThread())
	{
		for (const FInventoryListItem& Item : InventoryList.GetItems())
		{
			if (IsValid(Item.ItemInstance))
			{
				if (const UItemStaticData* ItemStaticData = Item.ItemInstance->GetItemStaticData())
				{
					GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Inventory Item - %s"), *ItemStaticData->GetItemName().ToString()));
				}
			}
		}
	}
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwnerRole() == ROLE_Authority)
	{
		for (const TSubclassOf<UItemStaticData>& Item : DefaultItems)
		{
			InventoryList.AddItem(Item);
		}
	}

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &ThisClass::GameplayEventCallback);
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::DropItemTag).AddUObject(this, &ThisClass::GameplayEventCallback);
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipNextTag).AddUObject(this, &ThisClass::GameplayEventCallback);
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::UnequipTag).AddUObject(this, &ThisClass::GameplayEventCallback);
	}
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (const FInventoryListItem& Item : InventoryList.GetItems())
	{
		if (IsValid(Item.ItemInstance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(Item.ItemInstance, *Bunch, *RepFlags);
		}
	}
	return bWroteSomething;
}

void UInventoryComponent::AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner() && GetOwner()->HasAuthority() && InItemStaticDataClass)
	{
		InventoryList.AddItem(InItemStaticDataClass);
	}
}

void UInventoryComponent::RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner() && GetOwner()-> HasAuthority() && InItemStaticDataClass)
	{
		InventoryList.RemoveItem(InItemStaticDataClass);
	}
}

void UInventoryComponent::EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		for (const FInventoryListItem& Item : InventoryList.GetItems())
		{
			if (Item.ItemInstance->ItemStaticDataClass == InItemStaticDataClass)
			{
				EquippedItem = Item.ItemInstance;
				EquippedItem->OnEquipped(GetOwner());
				break;
			}
		}
	}
}

void UInventoryComponent::UnequipItem()
{
	if (GetOwner() && GetOwner()->HasAuthority() && EquippedItem)
	{
		EquippedItem->OnUnequipped(GetOwner());
		EquippedItem = nullptr;
	}
}

void UInventoryComponent::DropItem()
{
	if (GetOwner() && GetOwner()->HasAuthority() && EquippedItem)
	{
		EquippedItem->OnDropped(GetOwner());
		RemoveItem(EquippedItem->ItemStaticDataClass);
		EquippedItem = nullptr;
	}
}

void UInventoryComponent::AddItemInstance(UInventoryItemInstance* InInventoryItemInstance)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(InInventoryItemInstance);
	}
}

void UInventoryComponent::EquipItemInstance(UInventoryItemInstance* InInventoryItemInstance)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		for (const FInventoryListItem& Item : InventoryList.GetItems())
		{
			if (Item.ItemInstance == InInventoryItemInstance)
			{
				Item.ItemInstance->OnEquipped(GetOwner());
				EquippedItem = Item.ItemInstance;
				break;
			}
		}
	}
}

void UInventoryComponent::EquipNext()
{
	const TArray<FInventoryListItem>& InventoryListItems = InventoryList.GetItems();
	const bool bNoItems = InventoryListItems.Num() == 0;
	const bool bOneAndEquipped = InventoryListItems.Num() == 1 && EquippedItem;

	if (bNoItems || bOneAndEquipped)
	{
		return;
	}

	UInventoryItemInstance* TargetItem = EquippedItem;
	for (const FInventoryListItem& Item : InventoryListItems)
	{
		if (Item.ItemInstance && Item.ItemInstance->GetItemStaticData() && Item.ItemInstance->GetItemStaticData()->CanBeEquipped())
		{
			if (Item.ItemInstance != EquippedItem)
			{
				TargetItem = Item.ItemInstance;
				break;
			}
		}
	}

	if (EquippedItem)
	{
		if (TargetItem == EquippedItem)
		{
			return;
		}
		UnequipItem();
	}

	EquipItemInstance(TargetItem);
}

void UInventoryComponent::AddInventoryTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	UInventoryComponent::EquipItemActorTag = TagsManager.AddNativeGameplayTag("Event.Inventory.EquipItemTag", "Equip item from item actor event.");
	UInventoryComponent::DropItemTag = TagsManager.AddNativeGameplayTag("Event.Inventory.DropItemTag", "Drop equipped item.");
	UInventoryComponent::EquipNextTag = TagsManager.AddNativeGameplayTag("Event.Inventory.EquipNextTag", "Try to equip next item.");
	UInventoryComponent::UnequipTag = TagsManager.AddNativeGameplayTag("Event.Inventory.UnequipTag", "Unequip current item.");

	TagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}

void UInventoryComponent::GameplayEventCallback(const FGameplayEventData* EventData)
{
	const ENetRole NetRole = GetOwnerRole();
	if (NetRole == ROLE_Authority)
	{
		HandleGameplayEventInternal(*EventData);
	}
	else if (NetRole == ROLE_AutonomousProxy)
	{
		Server_HandleGameplayEvent(*EventData);
	}
}

void UInventoryComponent::HandleGameplayEventInternal(FGameplayEventData EventData)
{
	const ENetRole NetRole = GetOwnerRole();
	if (NetRole == ROLE_Authority)
	{
		const FGameplayTag EventTag = EventData.EventTag;
		if (EventTag == UInventoryComponent::EquipItemActorTag)
		{
			if (UInventoryItemInstance* InventoryItemInstance = Cast<UInventoryItemInstance>(EventData.OptionalObject))
			{
				AddItemInstance(InventoryItemInstance);

				if (IsValid(EventData.Instigator))
				{
					const_cast<AActor*>(EventData.Instigator.Get())->Destroy();
				}
			}
		}
		else if (EventTag == UInventoryComponent::EquipNextTag)
		{
			EquipNext();
		}
		else if (EventTag == UInventoryComponent::DropItemTag)
		{
			DropItem();
		}
		else if (EventTag == UInventoryComponent::UnequipTag)
		{
			UnequipItem();
		}
	}
}

void UInventoryComponent::Server_HandleGameplayEvent_Implementation(FGameplayEventData EventData)
{
	HandleGameplayEventInternal(EventData);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
	DOREPLIFETIME(ThisClass, EquippedItem);
}
