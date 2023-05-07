// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "AGPhysicalMaterial.generated.h"

class USoundBase;

UCLASS()
class GASACTIONGAME_API UAGPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> FootstepSound;
};
