// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/ItemActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/InventoryItemInstance.h"
#include "Kismet/KismetSystemLibrary.h"

AItemActor::AItemActor()
{
 	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	InteractSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphereCollision"));
	InteractSphereCollision->SetSphereRadius(16.f);
	InteractSphereCollision->SetCollisionResponseToAllChannels(ECR_Block);
	InteractSphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractSphereCollision->SetupAttachment(GetRootComponent());
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	check(InteractSphereCollision);
	
	SetReplicateMovement(true);
	
	if (HasAuthority())
	{
		if (!IsValid(ItemInstance) && IsValid(ItemStaticDataClass))
		{
			ItemInstance = NewObject<UInventoryItemInstance>();
			ItemInstance->Init(ItemStaticDataClass);
		}

		InteractSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InteractSphereCollision->SetGenerateOverlapEvents(true);

		InteractSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractSphereBeginOverlap);
	}
	
}

bool AItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
	return WroteSomething;
}

void AItemActor::Init(UInventoryItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}

void AItemActor::OnEquipped()
{
	ItemState = EItemState::EIS_Equipped;
	if (InteractSphereCollision)
	{
		InteractSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InteractSphereCollision->SetGenerateOverlapEvents(false);
	}
}

void AItemActor::OnUnequipped()
{
	ItemState = EItemState::EIS_None;
	if (InteractSphereCollision)
	{
		InteractSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InteractSphereCollision->SetGenerateOverlapEvents(false);
	}
}

void AItemActor::OnDropped()
{
	const UWorld* World = GetWorld();
	if (Owner && World) // InventoryItemInstance
	{
		const FVector ItemLocation = GetActorLocation();
		const FVector OwnerForwardVector = Owner->GetActorForwardVector();

		constexpr float DropItemDistance = 100.f;
		constexpr float DropItemTraceHeight = 1000.f;

		const FVector TraceStart = ItemLocation + OwnerForwardVector * DropItemDistance;
		FVector TraceEnd = TraceStart - FVector::UpVector * DropItemTraceHeight;

		const IConsoleVariable* CVarShowDebug = IConsoleManager::Get().FindConsoleVariable(TEXT("AG.ShowDebugInventory"));
		const EDrawDebugTrace::Type DrawDebugTrace = CVarShowDebug->GetBool() ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		const TArray<AActor*> IgnoreActors {Owner};
		const FName TraceProfile = "WorldStatic";
		FHitResult HitResult;
		
		if (UKismetSystemLibrary::LineTraceSingleByProfile(World, TraceStart, TraceEnd, TraceProfile, true, IgnoreActors, DrawDebugTrace, HitResult, true))
		{
			if (HitResult.bBlockingHit)
			{
				TraceEnd = HitResult.ImpactPoint;
			}
		}
		
		GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		SetActorLocation(TraceEnd);

		ItemState = EItemState::EIS_Dropped;
		if (InteractSphereCollision)
		{
			InteractSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractSphereCollision->SetGenerateOverlapEvents(true);
		}
	}
}

void AItemActor::OnInteractSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		FGameplayEventData EventData;
		EventData.Instigator = this;
		EventData.OptionalObject = ItemInstance;
		EventData.EventTag = UInventoryComponent::EquipItemActorTag;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, UInventoryComponent::EquipItemActorTag, EventData);
	}
}

void AItemActor::OnRep_ItemState()
{
	if (!InteractSphereCollision)
	{
		return;
	}
	
	switch (ItemState)
	{
	case EItemState::EIS_Equipped:
		InteractSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InteractSphereCollision->SetGenerateOverlapEvents(false);
		
	default:
		InteractSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InteractSphereCollision->SetGenerateOverlapEvents(true);
	}
}

void AItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ItemInstance);
	DOREPLIFETIME(ThisClass, ItemState);
}
