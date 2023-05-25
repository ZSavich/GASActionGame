// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AGGameplayAbility_WallRun.h"

#include "AbilitySystemComponent.h"
#include "GASActionGameCharacter.h"
#include "AbilitySystem/Tasks/AbilityTask_TickWallRun.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAGGameplayAbility_WallRun::UAGGameplayAbility_WallRun()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAGGameplayAbility_WallRun::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (ActorInfo)
	{
		if (const ACharacter* CharacterAvatar = Cast<ACharacter>(ActorInfo->AvatarActor))
		{
			if (UCapsuleComponent* CapsuleComponent = CharacterAvatar->GetCapsuleComponent())
			{
				CapsuleComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnCapsuleComponentHit);
			}
		}
	}
}

void UAGGameplayAbility_WallRun::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	if (ActorInfo)
	{
		if (const ACharacter* CharacterAvatar = Cast<ACharacter>(ActorInfo->AvatarActor))
		{
			if (UCapsuleComponent* CapsuleComponent = CharacterAvatar->GetCapsuleComponent())
			{
				CapsuleComponent->OnComponentHit.RemoveDynamic(this, &ThisClass::OnCapsuleComponentHit);
			}
		}
	}
}

bool UAGGameplayAbility_WallRun::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const ACharacter* CharacterAvatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	return CharacterAvatar && CharacterAvatar->GetCharacterMovement() && !CharacterAvatar->GetCharacterMovement()->IsMovingOnGround();
}

void UAGGameplayAbility_WallRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WallRunTickTask = UAbilityTask_TickWallRun::CreateWallRunTask(this, Cast<ACharacter>(GetAvatarActorFromActorInfo()), Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent), TraceObjectTypes);
	WallRunTickTask->OnWallSideFinished.AddDynamic(this, &ThisClass::K2_EndAbility);
	WallRunTickTask->OnWallSideDetermined.AddDynamic(this, &ThisClass::OnWallSideDetermined);
	WallRunTickTask->ReadyForActivation();
}

void UAGGameplayAbility_WallRun::OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		AbilitySystemComponent->TryActivateAbility(GetCurrentAbilitySpec()->Handle);
	}
}

void UAGGameplayAbility_WallRun::OnWallSideDetermined(bool bLeftSide)
{
	const AGASActionGameCharacter* CharacterAvatar = Cast<AGASActionGameCharacter>(GetAvatarActorFromActorInfo());
	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	if (CharacterAvatar && AbilitySystemComponent)
	{
		const FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();

		if (bLeftSide)
		{
			CharacterAvatar->ApplyGameplayEffectToSelf(WallRunLeftSideEffectClass, EffectContextHandle);
		}
		else
		{
			CharacterAvatar->ApplyGameplayEffectToSelf(WallRunRightSideEffectClass, EffectContextHandle);
		}
	}
}

void UAGGameplayAbility_WallRun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (WallRunTickTask)
	{
		WallRunTickTask->EndTask();
	}

	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(WallRunLeftSideEffectClass, AbilitySystemComponent);
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(WallRunRightSideEffectClass, AbilitySystemComponent);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


