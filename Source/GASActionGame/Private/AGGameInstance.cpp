// Fill out your copyright notice in the Description page of Project Settings.

#include "AGGameInstance.h"
#include "AbilitySystemGlobals.h"

void UAGGameInstance::Init()
{
	Super::Init();

	// Load Global Tables and Tags for Ability System
	UAbilitySystemGlobals::Get().InitGlobalData();
}
