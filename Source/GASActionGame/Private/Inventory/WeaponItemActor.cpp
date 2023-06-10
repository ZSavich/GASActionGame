// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/WeaponItemActor.h"
#include "Inventory/InventoryItemInstance.h"

void AWeaponItemActor::InitInternal()
{
	Super::InitInternal();

	if (const UWeaponStaticData* WeaponStaticData = GetWeaponStaticData())
	{
		if (WeaponStaticData->SkeletalMesh)
		{
			if (USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), TEXT("WeaponMeshComponent")))
			{
				SkeletalMeshComponent->RegisterComponent();
				SkeletalMeshComponent->SetSkeletalMesh(WeaponStaticData->SkeletalMesh);
				SkeletalMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponStaticData->GetAttachmentSocketName());

				WeaponMeshComponent = SkeletalMeshComponent;
			}
		}
		else if (WeaponStaticData->StaticMesh)
		{
			if (UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("WeaponMeshComponent")))
			{
				StaticMeshComponent->RegisterComponent();
				StaticMeshComponent->SetStaticMesh(WeaponStaticData->StaticMesh);
				StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponStaticData->GetAttachmentSocketName());

				WeaponMeshComponent = StaticMeshComponent;
			}
		}
	}
}

UWeaponStaticData* AWeaponItemActor::GetWeaponStaticData() const
{
	return ItemInstance ? Cast<UWeaponStaticData>(ItemInstance->GetItemStaticData()) : nullptr;
}
