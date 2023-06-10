// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemActor.h"
#include "WeaponItemActor.generated.h"

UCLASS()
class GASACTIONGAME_API AWeaponItemActor : public AItemActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMeshComponent> WeaponMeshComponent;

public:
	virtual void InitInternal() override;

	UFUNCTION(BlueprintPure)
	FVector GetMuzzleLocation() const;
	
	UWeaponStaticData* GetWeaponStaticData() const;
};
