// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

class USphereComponent;
class UInventoryItemInstance;

UCLASS()
class GASACTIONGAME_API AItemActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	TObjectPtr<UInventoryItemInstance> ItemInstance;

	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	TEnumAsByte<EItemState> ItemState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> InteractSphereCollision;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;
	
public:	
	AItemActor();
	virtual void BeginPlay() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void Init(UInventoryItemInstance* InItemInstance);
	
	void OnEquipped();
	void OnUnequipped();
	void OnDropped();

	UFUNCTION()
	void OnInteractSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	UFUNCTION()
	void OnRep_ItemState();
};
