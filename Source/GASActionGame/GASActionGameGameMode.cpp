// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASActionGameGameMode.h"
#include "AGPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AGASActionGameGameMode::AGASActionGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = AAGPlayerController::StaticClass();
	
	RestartPlayerTime = 2.f;
}

void AGASActionGameGameMode::NotifyPlayerDied(AAGPlayerController* Controller) const
{
	if (Controller)
	{
		Controller->RestartPlayerIn(RestartPlayerTime);
	}
}
