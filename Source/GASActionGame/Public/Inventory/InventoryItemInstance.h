// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInstance.generated.h"

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

public:
	virtual void Init(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	
	UFUNCTION(BlueprintCallable)
	void OnEquipped();

	UFUNCTION(BlueprintCallable)
	void OnUnequipped();

	UItemStaticData* GetItemStaticData() const;
	
protected:
	UFUNCTION()
	void OnRep_IsEquipped();
	
};
