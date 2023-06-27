// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AGHUD.h"
#include "ViewModels/VMCharacterHealth.h"

void AAGHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	if (CharacterHealthViewModelClass)
	{
		CharacterHealthViewModel = NewObject<UVMCharacterHealth>(this, CharacterHealthViewModelClass);
	}
}
