// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AGFootstepsComponent.h"
#include "GASActionGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/AGPhysicalMaterial.h"

static TAutoConsoleVariable<bool> CVarShowFootsteps(
	TEXT("ShowDebugFootsteps"),
	false,
	TEXT("Display debug sphere and text about surface material and footstep line trace.\n")
	TEXT("False: Hide\n")
	TEXT("True: Display"),
	ECVF_Cheat
);

UAGFootstepsComponent::UAGFootstepsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	LeftFootSocketName = "SOCKET_Foot_L";
	RightFootSocketName = "SOCKET_Foot_R";
}

void UAGFootstepsComponent::HandleFootstep(const EFoot& Foot) const
{
	UWorld* World = GetWorld();
	AGASActionGameCharacter* OwnerCharacter = GetOwner<AGASActionGameCharacter>();
	
	if (World && OwnerCharacter)
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			const bool bShowDebug = CVarShowFootsteps.GetValueOnAnyThread();
			
			const FVector FootSocketLocation = CharacterMesh->GetSocketLocation(Foot == EF_Left ? LeftFootSocketName : RightFootSocketName);
			const FVector StartTrace = FootSocketLocation + FVector::UpVector * 20.f;
			const FVector EndTrace = StartTrace + FVector::UpVector * - 50.f;

			FHitResult HitResult;

			FCollisionQueryParams QueryParams;
			QueryParams.bReturnPhysicalMaterial = true;
			QueryParams.AddIgnoredActor(OwnerCharacter);
			
			if (World->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldStatic, QueryParams))
			{
				if (HitResult.bBlockingHit)
				{
					UAGPhysicalMaterial* PhysicalMaterial = Cast<UAGPhysicalMaterial>(HitResult.PhysMaterial.Get());
					
					if (PhysicalMaterial && PhysicalMaterial->FootstepSound)
					{
						UGameplayStatics::PlaySoundAtLocation(OwnerCharacter, PhysicalMaterial->FootstepSound, HitResult.ImpactPoint);
					}

					if (bShowDebug)
					{
						DrawDebugString(World, HitResult.ImpactPoint, FString::Printf(TEXT("Surface: %s"), *GetNameSafe(PhysicalMaterial)), nullptr, FColor::White, 5.f, true);
						DrawDebugSphere(World, HitResult.ImpactPoint, 5.f, 5, FColor::Green, false, 5.f, 0, 1.f);
					}
				}
				if (bShowDebug)
				{
					DrawDebugLine(World, StartTrace, EndTrace, FColor::Red, false, 5.f, 0, 1.f);
				}
			}
		}
	}
}
