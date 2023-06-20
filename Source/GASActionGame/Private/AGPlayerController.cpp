// Fill out your copyright notice in the Description page of Project Settings.

#include "AGPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GASActionGameGameMode.h"

void AAGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InPawn))
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("State.Dead", EGameplayTagEventType::NewOrRemoved)).AddUObject(this, &AAGPlayerController::OnPawnDeathStateChanged);
	} 
}

void AAGPlayerController::OnUnPossess()
{
	if (GetPawn())
	{
		if (DeathStateTagDelegate.IsValid())
		{
			if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()))
			{
				AbilitySystemComponent->UnregisterGameplayTagEvent(DeathStateTagDelegate, FGameplayTag::RequestGameplayTag("State.Dead", EGameplayTagEventType::NewOrRemoved));
			}
		}
	}
	Super::OnUnPossess();
}

void AAGPlayerController::RestartPlayerIn(float InTime)
{
	ChangeState(EName::Spectating);

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(Timer_RestartPlayer, this, &ThisClass::RestartPlayer, InTime, false);
	}
}

void AAGPlayerController::OnPawnDeathStateChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		if (const UWorld* World = GetWorld())
		{
			if (AGASActionGameGameMode* GameMode = World->GetAuthGameMode<AGASActionGameGameMode>())
			{
				GameMode->NotifyPlayerDied(this);
			}
		}
	}
}

void AAGPlayerController::RestartPlayer()
{
	if (const UWorld* World = GetWorld())
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			GameMode->RestartPlayer(this);
		}
	}
}
