// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AGHUD.generated.h"

class UVMCharacterHealth;

UCLASS()
class GASACTIONGAME_API AAGHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVMCharacterHealth> CharacterHealthViewModelClass;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UVMCharacterHealth> CharacterHealthViewModel;
	
public:
	virtual void PreInitializeComponents() override;
};
