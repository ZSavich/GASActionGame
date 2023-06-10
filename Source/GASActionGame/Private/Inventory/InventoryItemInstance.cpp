// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemInstance.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AGTypes.h"
#include "InventoryFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Inventory/ItemActor.h"
#include "Net/UnrealNetwork.h"

void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

void UInventoryItemInstance::OnEquipped(AActor* InOwner)
{
	if (UWorld* World = InOwner->GetWorld())
	{
		if (const UItemStaticData* ItemStaticData = GetItemStaticData())
		{
			if (ItemStaticData->GetItemActorClass())
			ItemActor = World->SpawnActorDeferred<AItemActor>(ItemStaticData->GetItemActorClass(), FTransform::Identity, InOwner);
			if (ItemActor)
			{
				ItemActor->Init(this);
				ItemActor->FinishSpawning(FTransform::Identity);
				
				bIsEquipped = true;
				ItemActor->OnEquipped();
				
				if (const ACharacter* OwnerCharacter = Cast<ACharacter>(InOwner))
				{
					if (USkeletalMeshComponent* SkeletalMeshComponent = OwnerCharacter->GetMesh())
					{
						ItemActor->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemStaticData->GetAttachmentSocketName());
					}
				}

				TryGrantAbilities(InOwner);
			}
		}
	}
}

void UInventoryItemInstance::OnUnequipped(AActor* InOwner)
{
	if (ItemActor)
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
		bIsEquipped = false;
	}

	TryRemoveAbilities(InOwner);
}

void UInventoryItemInstance::OnDropped(AActor* InOwner)
{
	if (ItemActor)
	{
		ItemActor->OnDropped();
		bIsEquipped = false;
	}

	TryRemoveAbilities(InOwner);
}

UItemStaticData* UInventoryItemInstance::GetItemStaticData() const
{
	return UInventoryFunctionLibrary::GetItemStaticData(ItemStaticDataClass);
}

void UInventoryItemInstance::TryGrantAbilities(AActor* InOwner)
{
	if (InOwner && InOwner->HasAuthority())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
		{
			if (UItemStaticData* ItemStaticData = GetItemStaticData())
			{
				for (const TSubclassOf<UGameplayAbility>& Ability : ItemStaticData->GrantedAbilities)
				{
					FGameplayAbilitySpecHandle AbilitySpec = AbilitySystemComponent->K2_GiveAbility(Ability);
					GrantedAbilities.Add(AbilitySpec);
				}
			}
		}
	}
}

void UInventoryItemInstance::TryRemoveAbilities(AActor* InOwner)
{
	if (InOwner && InOwner->HasAuthority())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
		{
			for (FGameplayAbilitySpecHandle AbilityHandle : GrantedAbilities)
			{
				AbilitySystemComponent->ClearAbility(AbilityHandle);
			}
			GrantedAbilities.Empty();
		}
	}
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