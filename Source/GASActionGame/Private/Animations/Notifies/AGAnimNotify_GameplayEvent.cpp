// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/Notifies/AGAnimNotify_GameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"

void UAGAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventData.EventTag, EventData);
}
