// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InventoryComponent.h"

#include "AGTypes.h"
#include "Inventory/InventoryItemInstance.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

TAutoConsoleVariable<bool> CVarShowDebugInventory(TEXT("AG.ShowDebugInventory"), false, TEXT("Print information about inventory in output log."), ECVF_Cheat);

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
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

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}