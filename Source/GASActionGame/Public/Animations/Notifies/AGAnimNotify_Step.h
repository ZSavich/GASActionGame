// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AGAnimNotify_Step.generated.h"

UCLASS()
class GASACTIONGAME_API UAGAnimNotify_Step : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	TEnumAsByte<EFoot> Foot;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
