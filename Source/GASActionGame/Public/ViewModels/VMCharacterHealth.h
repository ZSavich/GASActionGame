// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VMCharacterHealth.generated.h"

UCLASS(BlueprintType)
class GASACTIONGAME_API UVMCharacterHealth : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 CurrentHealth;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 MaxHealth;
	
	void SetCurrentHealth(int32 NewCurrentHealth);
	void SetMaxHealth(int32 NewMaxHealth);

private:
	int32 GetCurrentHealth() const { return CurrentHealth; }
	int32 GetMaxHealth() const { return MaxHealth; }

public:
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercent() const
	{
		return MaxHealth == 0 ? 0 : (float)CurrentHealth / (float)MaxHealth;
	}
};
