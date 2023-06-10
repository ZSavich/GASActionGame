// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "InventoryItemInstance.generated.h"

class AItemActor;
class UItemStaticData;

UCLASS()
class GASACTIONGAME_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

public:
	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;

	UPROPERTY(ReplicatedUsing = "OnRep_IsEquipped")
	bool bIsEquipped;

protected:
	UPROPERTY(Replicated)
	TObjectPtr<AItemActor> ItemActor;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;

public:
	virtual void Init(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	
	void OnEquipped(AActor* InOwner = nullptr);
	void OnUnequipped(AActor* InOwner = nullptr);
	void OnDropped(AActor* InOwner = nullptr);

	UItemStaticData* GetItemStaticData() const;
	AItemActor* GetItemActor() const;

	void TryGrantAbilities(AActor* InOwner);
	void TryRemoveAbilities(AActor* InOwner);
	
protected:
	UFUNCTION()
	void OnRep_IsEquipped();
	
};
