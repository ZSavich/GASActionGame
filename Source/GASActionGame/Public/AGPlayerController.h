// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AGPlayerController.generated.h"

UCLASS()
class GASACTIONGAME_API AAGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	void RestartPlayerIn(float InTime);
	
protected:
	void RestartPlayer();

	UFUNCTION()
	void OnPawnDeathStateChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:
	FTimerHandle Timer_RestartPlayer;
	FDelegateHandle DeathStateTagDelegate;
};
