// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.h"
#include "Engine/DataAsset.h"
#include "AGCharacterAnimDataAsset.generated.h"

UCLASS()
class GASACTIONGAME_API UAGCharacterAnimDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	FCharacterAnimData CharacterAnimationData;
	
};
