// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryList.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASACTIONGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Replicated)
	FInventoryList InventoryList;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticData>> DefaultItems;

	UPROPERTY(Replicated)
	TObjectPtr<UInventoryItemInstance> EquippedItem;
	
public:	
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	void AddItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);
	void RemoveItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	UFUNCTION(BlueprintCallable)
	void DropItem();

	FORCEINLINE UInventoryItemInstance* GetEquippedItem() const { return EquippedItem; }
};
