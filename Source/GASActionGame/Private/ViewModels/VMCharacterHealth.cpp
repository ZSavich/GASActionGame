// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewModels/VMCharacterHealth.h"

void UVMCharacterHealth::SetCurrentHealth(int32 NewCurrentHealth)
{
	if (CurrentHealth != NewCurrentHealth)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, NewCurrentHealth);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}

void UVMCharacterHealth::SetMaxHealth(int32 NewMaxHealth)
{
	if (MaxHealth != NewMaxHealth)
	{
		UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}
