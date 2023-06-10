// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemInstance.h"

#include "AGTypes.h"
#include "InventoryFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Inventory/ItemActor.h"
#include "Net/UnrealNetwork.h"

void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

void UInventoryItemInstance::OnEquipped(AActor* OwnerActor)
{
	if (UWorld* World = OwnerActor->GetWorld())
	{
		if (const UItemStaticData* ItemStaticData = GetItemStaticData())
		{
			if (ItemStaticData->GetItemActorClass())
			ItemActor = World->SpawnActorDeferred<AItemActor>(ItemStaticData->GetItemActorClass(), FTransform::Identity, OwnerActor);
			if (ItemActor)
			{
				ItemActor->Init(this);
				ItemActor->FinishSpawning(FTransform::Identity);
				
				bIsEquipped = true;
				ItemActor->OnEquipped();
				
				if (const ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor))
				{
					if (USkeletalMeshComponent* SkeletalMeshComponent = OwnerCharacter->GetMesh())
					{
						ItemActor->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemStaticData->GetAttachmentSocketName());
					}
				}
			}
		}
	}
}

void UInventoryItemInstance::OnUnequipped()
{
	if (ItemActor)
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
		bIsEquipped = false;
	}
}

void UInventoryItemInstance::OnDropped()
{
	if (ItemActor)
	{
		ItemActor->OnDropped();
		bIsEquipped = false;
	}
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
	DOREPLIFETIME(ThisClass, ItemActor);
}