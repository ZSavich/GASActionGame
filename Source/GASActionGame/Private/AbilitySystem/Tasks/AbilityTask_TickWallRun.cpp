// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Tasks/AbilityTask_TickWallRun.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAbilityTask_TickWallRun::Activate()
{
	Super::Activate();

	FHitResult WallHitResult;
	if (!FindRunnableWall(WallHitResult))
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnWallSideFinished.Broadcast();
		}
		EndTask();
		return;
	}

	OnWallSideDetermined.Broadcast(IsWallOnTheLeft(WallHitResult));
	CharacterOwner->Landed(WallHitResult);
	CharacterOwner->SetActorLocation(WallHitResult.ImpactPoint + WallHitResult.ImpactNormal * 60.f);
	CharacterMovementComponent->SetMovementMode(MOVE_Flying);
}

UAbilityTask_TickWallRun* UAbilityTask_TickWallRun::CreateWallRunTask(UGameplayAbility* OwningAbility, ACharacter* InCharacterOwner, UCharacterMovementComponent*  InCharacterMovement, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes)
{
	UAbilityTask_TickWallRun* WallRunTask = NewAbilityTask<UAbilityTask_TickWallRun>(OwningAbility);

	WallRunTask->CharacterOwner = InCharacterOwner;
	WallRunTask->CharacterMovementComponent = InCharacterMovement;
	WallRunTask->bTickingTask = true;
	WallRunTask->TraceObjectTypes = TraceObjectTypes;

	return WallRunTask;
}

void UAbilityTask_TickWallRun::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	FHitResult WallHitResult;

	if (!FindRunnableWall(WallHitResult))
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnWallSideFinished.Broadcast();
		}
		EndTask();
		return;
	}

	const FRotator DirectionRotator = IsWallOnTheLeft(WallHitResult) ? FRotator(0, -90, 0) : FRotator(0, 90, 0);
	const FVector WallRunDirection = DirectionRotator.RotateVector(WallHitResult.ImpactNormal);

	CharacterMovementComponent->Velocity = WallRunDirection * 700.f;
	CharacterMovementComponent->Velocity.Z = 0;
	CharacterOwner->SetActorRotation(WallRunDirection.Rotation());
	CharacterMovementComponent->SetPlaneConstraintEnabled(true);
	CharacterMovementComponent->SetPlaneConstraintOrigin(WallHitResult.ImpactPoint);
	CharacterMovementComponent->SetPlaneConstraintNormal(WallHitResult.ImpactNormal);
}

bool UAbilityTask_TickWallRun::FindRunnableWall(FHitResult& OutHitResult)
{
	const FVector CharacterLocation = CharacterOwner->GetActorLocation();
	const FVector RightVector = CharacterOwner->GetActorRightVector();
	const FVector ForwardVector = CharacterOwner->GetActorForwardVector();

	const float TraceLength = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + 30.f;

	TArray<AActor*> ActorsToIgnore = {CharacterOwner};

	static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("AG.ShowDebugTraversal"));
	const bool bShowTraversal = CVar->GetInt() > 0;

	EDrawDebugTrace::Type DrawDebugType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + ForwardVector * TraceLength, TraceObjectTypes, true, ActorsToIgnore, DrawDebugType, OutHitResult, true))
	{
		return false;
	}

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + -RightVector * TraceLength, TraceObjectTypes, true, ActorsToIgnore, DrawDebugType, OutHitResult, true))
	{
		if (FVector::DotProduct(OutHitResult.ImpactNormal, RightVector) > 0.3f)
		{
			return true;
		}
	}

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + RightVector * TraceLength, TraceObjectTypes, true, ActorsToIgnore, DrawDebugType, OutHitResult, true))
	{
		if(FVector::DotProduct(OutHitResult.ImpactNormal, -RightVector) > 0.3f)
		{
			return true;
		}
	}

	return false;
}

bool UAbilityTask_TickWallRun::IsWallOnTheLeft(const FHitResult& InWallHitResult)
{
	return FVector::DotProduct(CharacterOwner->GetActorRightVector(), InWallHitResult.ImpactNormal) > 0;
}

void UAbilityTask_TickWallRun::OnDestroy(bool bInOwnerFinished)
{
	CharacterMovementComponent->SetPlaneConstraintEnabled(false);
	CharacterMovementComponent->SetMovementMode(MOVE_Falling);
	
	Super::OnDestroy(bInOwnerFinished);
}
