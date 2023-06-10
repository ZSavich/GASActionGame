// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryList.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASACTIONGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static FGameplayTag EquipItemActorTag;
	static FGameplayTag DropItemTag;
	static FGameplayTag EquipNextTag;
	static FGameplayTag UnequipTag;
	
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

	void AddItemInstance(UInventoryItemInstance* InInventoryItemInstance);
	void EquipItemInstance(UInventoryItemInstance* InInventoryItemInstance);
	void EquipNext();

	void GameplayEventCallback(const FGameplayEventData* EventData);

	void AddInventoryTags();

	void HandleGameplayEventInternal(FGameplayEventData EventData);

	UFUNCTION(Server, Reliable)
	void Server_HandleGameplayEvent(FGameplayEventData EventData);
	
	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	UFUNCTION(BlueprintCallable)
	void DropItem();

	FORCEINLINE UInventoryItemInstance* GetEquippedItem() const { return EquippedItem; }
};
