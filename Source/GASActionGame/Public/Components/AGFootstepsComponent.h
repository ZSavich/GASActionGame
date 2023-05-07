// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.h"
#include "Components/ActorComponent.h"
#include "AGFootstepsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASACTIONGAME_API UAGFootstepsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName LeftFootSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName RightFootSocketName;
	
public:	
	UAGFootstepsComponent();

	void HandleFootstep(const EFoot& Foot) const;
		
};
