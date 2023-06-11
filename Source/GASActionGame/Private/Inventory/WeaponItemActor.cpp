// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/WeaponItemActor.h"
#include "Inventory/InventoryItemInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/AGPhysicalMaterial.h"
#include "NiagaraFunctionLibrary.h"

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

FVector AWeaponItemActor::GetMuzzleLocation() const
{
	return WeaponMeshComponent ? WeaponMeshComponent->GetSocketLocation(GetWeaponStaticData()->MuzzleSocketName) : FVector::Zero();
}

UWeaponStaticData* AWeaponItemActor::GetWeaponStaticData() const
{
	return ItemInstance ? Cast<UWeaponStaticData>(ItemInstance->GetItemStaticData()) : nullptr;
}

void AWeaponItemActor::PlayWeaponEffects(const FHitResult& InHitResult)
{
	if (HasAuthority())
	{
		Multicast_PlayWeaponEffects(InHitResult);
	}
	else
	{
		PlayWeaponEffectsInternal(InHitResult);
	}
}

void AWeaponItemActor::Multicast_PlayWeaponEffects_Implementation(const FHitResult& InHitResult)
{
	if (!Owner || Owner->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		PlayWeaponEffectsInternal(InHitResult);
	}
}

void AWeaponItemActor::PlayWeaponEffectsInternal(const FHitResult& InHitResult) const
{
	if (InHitResult.PhysMaterial.IsValid())
	{
		if (const UAGPhysicalMaterial* PhysicMaterial = Cast<UAGPhysicalMaterial>(InHitResult.PhysMaterial.Get()))
		{
			UGameplayStatics::PlaySoundAtLocation(this, PhysicMaterial->PointImpactSound, InHitResult.ImpactPoint);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PhysicMaterial->PointImpactVFX, InHitResult.ImpactPoint);
		}

		if (const UWeaponStaticData* WeaponStaticData = GetWeaponStaticData())
		{
			UGameplayStatics::PlaySoundAtLocation(this, WeaponStaticData->AttackSound, GetMuzzleLocation());
		}
	}
}
