// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASActionGameCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemLog.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AGAttributeSetBase.h"
#include "AbilitySystem/Components/AGAbilitySystemComponent.h"
#include "Components/AGCharacterMovementComponent.h"
#include "Components/AGFootstepsComponent.h"
#include "Components/AGMotionWarpingComponent.h"
#include "Components/InventoryComponent.h"
#include "DataAssets/AGCharacterDataAsset.h"
#include "Net/UnrealNetwork.h"

AGASActionGameCharacter::AGASActionGameCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UAGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // Replicate minimum info to simulate proxies and maximum info to owner and autonomous proxies

	AttributeSet = CreateDefaultSubobject<UAGAttributeSetBase>("AttributeSet");

	FootstepsComponent = CreateDefaultSubobject<UAGFootstepsComponent>("FootstepsComponent");

	MotionWarpingComponent = CreateDefaultSubobject<UAGMotionWarpingComponent>("MotionWarpingComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

UAbilitySystemComponent* AGASActionGameCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASActionGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxMovementSpeedAttribute()).AddUObject(this, &AGASActionGameCharacter::HandleOnMaxMovementSpeedChanged);
	}
}

void AGASActionGameCharacter::PostLoad()
{
	Super::PostLoad();

	if (CharacterDataAsset)
	{
		SetCharacterData(CharacterDataAsset->CharacterData);
	}
}

void AGASActionGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(AbilitySystemComponent)
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	GiveAbilities();
	ApplyStartupEffects();
	
}

void AGASActionGameCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	check(AbilitySystemComponent)
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AGASActionGameCharacter::OnRep_CharacterData()
{
	InitFromCharacterData(CharacterData, true);
}

void AGASActionGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGASActionGameCharacter::Input_Jump);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGASActionGameCharacter::Input_Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASActionGameCharacter::Input_Look);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGASActionGameCharacter::Input_CrouchStart);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AGASActionGameCharacter::Input_CrouchEnd);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGASActionGameCharacter::Input_SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGASActionGameCharacter::Input_SprintEnded);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &AGASActionGameCharacter::Input_Drop);
		EnhancedInputComponent->BindAction(EquipNextAction, ETriggerEvent::Started, this, &AGASActionGameCharacter::Input_EquipNext);
		EnhancedInputComponent->BindAction(UnequipAction, ETriggerEvent::Started, this, &AGASActionGameCharacter::Input_Unequip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AGASActionGameCharacter::Input_AttackStart);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AGASActionGameCharacter::Input_AttackEnd);
	}
}

void AGASActionGameCharacter::Input_Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGASActionGameCharacter::Input_Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGASActionGameCharacter::Input_Jump(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		if (UAGCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement<UAGCharacterMovementComponent>())
		{
			CharacterMovementComponent->TryTraversal(AbilitySystemComponent);
		}
	}
}

void AGASActionGameCharacter::Input_CrouchStart(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(CrouchTags);
	}
}

void AGASActionGameCharacter::Input_CrouchEnd(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities(&CrouchTags);
	}
}

void AGASActionGameCharacter::Input_SprintStart(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(SprintTags);
	}
}

void AGASActionGameCharacter::Input_SprintEnded(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities(&SprintTags);
	}
}

void AGASActionGameCharacter::Input_Drop(const FInputActionValue& Value)
{
	FGameplayEventData EventData;
	EventData.EventTag = UInventoryComponent::DropItemTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::DropItemTag, EventData);
}

void AGASActionGameCharacter::Input_EquipNext(const FInputActionValue& Value)
{
	FGameplayEventData EventData;
	EventData.EventTag = UInventoryComponent::EquipNextTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::EquipNextTag, EventData);
}

void AGASActionGameCharacter::Input_Unequip(const FInputActionValue& Value)
{
	FGameplayEventData EventData;
	EventData.EventTag = UInventoryComponent::UnequipTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::UnequipTag, EventData);
}

void AGASActionGameCharacter::Input_AttackStart(const FInputActionValue& Value)
{
	FGameplayEventData EventData;
	EventData.EventTag = AttackStartTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, AttackStartTag, EventData);
}

void AGASActionGameCharacter::Input_AttackEnd(const FInputActionValue& Value)
{
	FGameplayEventData EventData;
	EventData.EventTag = AttackEndTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, AttackEndTag, EventData);
}

void AGASActionGameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveActiveEffectsWithTags(InAirTags);
	}
}

void AGASActionGameCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	if (AbilitySystemComponent && CrouchStateEffect)
	{
		const FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		if (EffectContext.IsValid())
		{
			const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CrouchStateEffect, 1.f, EffectContext);
			if (SpecHandle.IsValid())
			{
				const FActiveGameplayEffectHandle ActiveEffect = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if (!ActiveEffect.WasSuccessfullyApplied())
				{
					ABILITY_LOG(Error, TEXT("Failed to apply the %s gameplay effect to the character."), *GetNameSafe(CrouchStateEffect));
				}
			}
		}
	}
}

void AGASActionGameCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	if (AbilitySystemComponent && CrouchStateEffect)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(CrouchStateEffect, AbilitySystemComponent);
	}
}

bool AGASActionGameCharacter::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext) const
{
	check(AbilitySystemComponent)
	if (Effect.Get() == nullptr) return false;
	
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, InEffectContext);
	if (SpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		return ActiveGEHandle.WasSuccessfullyApplied();
	}
	return false;
}

void AGASActionGameCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent && !CharacterData.Abilities.IsEmpty())
	{
		for (const TSubclassOf<UGameplayAbility> Ability : CharacterData.Abilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
		}
	}
}

void AGASActionGameCharacter::ApplyStartupEffects()
{
	if (HasAuthority() && AbilitySystemComponent && !CharacterData.Effects.IsEmpty())
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (const TSubclassOf<UGameplayEffect> Effect : CharacterData.Effects)
		{
			ApplyGameplayEffectToSelf(Effect, EffectContext);
		}
	}
}

void AGASActionGameCharacter::InitFromCharacterData(const FCharacterData& InCharacterData, const bool bFromReplication)
{
	// ToDo
}

void AGASActionGameCharacter::SetCharacterData(const FCharacterData& InCharacterData)
{
	CharacterData = InCharacterData;
	InitFromCharacterData(CharacterData);
}

void AGASActionGameCharacter::HandleOnMaxMovementSpeedChanged(const FOnAttributeChangeData& MaxMovementSpeedAttribute)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxMovementSpeedAttribute.NewValue;
	}
}

void AGASActionGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CharacterData);
	DOREPLIFETIME(ThisClass, InventoryComponent);
}