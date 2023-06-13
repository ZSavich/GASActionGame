// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GASActionGameGameMode.generated.h"

class AAGPlayerController;

UCLASS(minimalapi)
class AGASActionGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action Game")
	float RestartPlayerTime;
	
public:
	AGASActionGameGameMode();

	void NotifyPlayerDied(AAGPlayerController* Controller) const;
};



