// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.h"
#include "Engine/DataAsset.h"
#include "AGCharacterDataAsset.generated.h"

UCLASS()
class GASACTIONGAME_API UAGCharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCharacterData CharacterData;
};
