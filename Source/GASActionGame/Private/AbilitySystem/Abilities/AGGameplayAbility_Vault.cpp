// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AGGameplayAbility_Vault.h"

#include "GASActionGameCharacter.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/AGMotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UAGGameplayAbility_Vault::UAGGameplayAbility_Vault()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	HorizontalTraceRadius = 30.f;
	HorizontalTraceLength = 500.f;
	HorizontalTraceCount = 5.f;
	HorizontalTraceStep = 60.f;
	VerticalTraceRadius = 30.f;
	VerticalTraceStep = 30.f;
}

bool UAGGameplayAbility_Vault::CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}
	
	AGASActionGameCharacter* Character = GetActionGameCharacterFromActorInfo();
	if (!Character)
	{
		return false;
	}

	FVector StartLocation = Character->GetActorLocation();
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector UpVector = Character->GetActorUpVector();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	const auto bShowTraversal = IConsoleManager::Get().FindConsoleVariable(TEXT("AG.ShowDebugTraversal"));
	EDrawDebugTrace::Type DrawDebugType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	bool bJumpToLocationSet = false;
	int32 JumpToLocationIndex = INDEX_NONE;
	int32 Index = 0;

	FHitResult TraceResult;
	float MaxJumpDistance = HorizontalTraceLength;

	for (; Index < HorizontalTraceCount; ++Index)
	{
		FVector TraceStart = StartLocation + Index * UpVector * HorizontalTraceStep;
		FVector TraceEnd = TraceStart + ForwardVector * HorizontalTraceLength;

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TraceStart, TraceEnd, HorizontalTraceRadius, TraceObjectTypes, true, ActorsToIgnore, DrawDebugType, TraceResult, true))
		{
			if (JumpToLocationIndex == INDEX_NONE && (Index < HorizontalTraceCount - 1))
			{
				JumpToLocationIndex = Index;
				JumpToLocation = TraceResult.Location;
			}
			else
			{
				MaxJumpDistance = FVector::Dist2D(TraceResult.Location, TraceStart);
				break;
			}
		}
		else if (JumpToLocationIndex != INDEX_NONE)
		{
			break;
		}
	}

	if (JumpToLocationIndex == INDEX_NONE)
	{
		return false;
	}

	const float DistanceToJumpTo = FVector::Dist2D(StartLocation, JumpToLocation);
	const float MaxVerticalTraceDistance = MaxJumpDistance - DistanceToJumpTo;

	if (MaxVerticalTraceDistance < 0.f)
	{
		return false;
	}

	if (Index == HorizontalTraceCount)
	{
		Index = HorizontalTraceCount - 1;
	}

	const float VerticalTraceLength = FMath::Abs(JumpToLocation.Z - (StartLocation + Index * UpVector * HorizontalTraceStep).Z);
	FVector VerticalStartLocation = JumpToLocation + UpVector * VerticalTraceLength;

	Index = 0;

	const float VerticalTraceCount = MaxVerticalTraceDistance / VerticalTraceStep;
	bool bJumpOverLocationSet = false;

	for (; Index <= VerticalTraceCount; ++Index)
	{
		const FVector TraceStart = VerticalStartLocation + Index * ForwardVector * VerticalTraceStep;
		const FVector TraceEnd = TraceStart + UpVector * VerticalTraceLength * -1;

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TraceStart,TraceEnd, VerticalTraceRadius, TraceObjectTypes, true, ActorsToIgnore, DrawDebugType, TraceResult, true))
		{
			JumpOverLocation = TraceResult.ImpactPoint;

			if (Index == 0)
			{
				JumpToLocation = JumpOverLocation;
			}
		}
		else if (Index != 0)
		{
			bJumpOverLocationSet = true;
			break;
		}
	}

	if (!bJumpOverLocationSet)
	{
		return false;
	}

	const FVector TraceStart = JumpOverLocation + ForwardVector * VerticalTraceStep;
	const FVector TraceEnd = TraceStart + UpVector * VerticalTraceLength * -1;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TraceStart, TraceEnd, VerticalTraceRadius, TraceObjectTypes, true, ActorsToIgnore, DrawDebugType, TraceResult, true))
	{
		JumpOverLocation = TraceResult.ImpactPoint;
	}

	if (bShowTraversal)
	{
		DrawDebugSphere(GetWorld(), JumpToLocation, 15.f, 16, FColor::White, false, 7);
		DrawDebugSphere(GetWorld(), JumpOverLocation, 15.f, 16, FColor::White, false, 7);
	}
	return true;
}

void UAGGameplayAbility_Vault::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		K2_EndAbility();
		return;
	}

	if (AGASActionGameCharacter* Character = GetActionGameCharacterFromActorInfo())
	{
		UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
		UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
		UMotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent();
		
		if (!CharacterMovement || !CapsuleComponent || !MotionWarpingComponent)
		{
			K2_EndAbility();
			return;
		}

		CharacterMovement->SetMovementMode(MOVE_Flying);
		
		for (TEnumAsByte<ECollisionChannel>& Channel : CollisionChannelsToIgnore)
		{
			CapsuleComponent->SetCollisionResponseToChannel(Channel, ECR_Ignore);
		}

		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation("JumpToLocation", JumpToLocation, Character->GetActorRotation());
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation("JumpOverLocation", JumpOverLocation, Character->GetActorRotation());

		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, VaultMontage);

		MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		MontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		MontageTask->ReadyForActivation();
	}
}

void UAGGameplayAbility_Vault::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (MontageTask)
	{
		MontageTask->EndTask();
	}

	if (AGASActionGameCharacter* Character = GetActionGameCharacterFromActorInfo())
	{
		UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
		UMotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent();
		UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();

		if (CapsuleComponent && MotionWarpingComponent && CharacterMovementComponent)
		{
			for (TEnumAsByte<ECollisionChannel> Channel : CollisionChannelsToIgnore)
			{
				CapsuleComponent->SetCollisionResponseToChannel(Channel, ECR_Block);
			}

			MotionWarpingComponent->RemoveWarpTarget("JumpToLocation");
			MotionWarpingComponent->RemoveWarpTarget("JumpOverLocation");

			if (CharacterMovementComponent->IsFlying())
			{
				CharacterMovementComponent->SetMovementMode(MOVE_Falling);
			}
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
