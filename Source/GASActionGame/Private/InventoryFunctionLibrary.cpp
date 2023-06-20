// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AGTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Projectiles/Projectile.h"

TAutoConsoleVariable<bool> CVarShowRadialDamageDebug(TEXT("AG.ShowRadialDamageDebug"), false, TEXT("Show information about radial damage."), ECVF_Cheat);

UItemStaticData* UInventoryFunctionLibrary::GetItemStaticData(const TSubclassOf<UItemStaticData> ItemDataClass)
{
	if (IsValid(ItemDataClass))
	{
		return ItemDataClass->GetDefaultObject<UItemStaticData>();
	}
	return nullptr;
}

void UInventoryFunctionLibrary::ApplyRadialDamage(const UObject* WorldContextObject, AActor* DamageCauser, FVector Location, float DamageRadius, float DamageAmount, TArray<TSubclassOf<UGameplayEffect>> DamageEffects, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, TEnumAsByte<ETraceTypeQuery> TraceType)
{
	TArray<AActor*> OutActors;
	TArray<AActor*> ActorsToIgnore {DamageCauser};

	UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, Location, DamageRadius, ObjectTypes, nullptr, ActorsToIgnore, OutActors);

	const bool bShowDebug = CVarShowRadialDamageDebug.GetValueOnAnyThread();

	for (const AActor* Actor : OutActors)
	{
		FHitResult HitResult;
		if (UKismetSystemLibrary::LineTraceSingle(WorldContextObject, Location, Actor->GetActorLocation(), TraceType, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true))
		{
			AActor* TargetActor = HitResult.GetActor();
			if (TargetActor == Actor)
			{
				bool bWasApplied = false;
				if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
				{
					FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
					EffectContext.AddInstigator(DamageCauser, DamageCauser);

					for (TSubclassOf<UGameplayEffect> Effect : DamageEffects)
					{
						FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, EffectContext);
						if (SpecHandle.IsValid())
						{
							UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FGameplayTag::RequestGameplayTag("Attribute.Health"), -DamageAmount);
							FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
							bWasApplied = ActiveEffectHandle.WasSuccessfullyApplied();
						}
					}
				}
				
				if (bShowDebug)
				{
					DrawDebugLine(WorldContextObject->GetWorld(), Location, TargetActor->GetActorLocation(), bWasApplied ? FColor::Green : FColor::Red, false, 4.f, 0, 1.f);
					DrawDebugSphere(WorldContextObject->GetWorld(), HitResult.Location, 16.f, 16, bWasApplied ? FColor::Green : FColor::Red, false, 4.f, 0, 1.f);
					DrawDebugString(WorldContextObject->GetWorld(), HitResult.Location, *GetNameSafe(TargetActor), nullptr, FColor::White, 0, false, 1.f);
				}
			}
			else
			{
				if (bShowDebug)
				{
					DrawDebugLine(WorldContextObject->GetWorld(), Location, TargetActor->GetActorLocation(), FColor::Red, false, 4.f, 0, 1.f);
					DrawDebugSphere(WorldContextObject->GetWorld(), HitResult.Location, 16.f, 16, FColor::Red, false, 4.f, 0, 1.f);
					DrawDebugString(WorldContextObject->GetWorld(), HitResult.Location, *GetNameSafe(TargetActor), nullptr, FColor::Red, 0, false, 1.f);
				}
			}
		}
		else
		{
			if (bShowDebug)
			{
				DrawDebugLine(WorldContextObject->GetWorld(), Location, Actor->GetActorLocation(), FColor::Red, false, 4.f, 0, 1.f);
				DrawDebugSphere(WorldContextObject->GetWorld(), HitResult.Location, 16.f, 16, FColor::Red, false, 4.f, 0, 1.f);
				DrawDebugString(WorldContextObject->GetWorld(), HitResult.Location, GetNameSafe(Actor), nullptr, FColor::Red, 0, false, 1.f);
			}
		}
	}

	if (bShowDebug)
	{
		DrawDebugSphere(WorldContextObject->GetWorld(), Location, DamageRadius, 16, FColor::White, false, 4.f, 0, 1.f);
	}
}

AProjectile* UInventoryFunctionLibrary::LaunchProjectile(const UObject* WorldContextObject, TSubclassOf<UProjectileStaticData> ProjectileStaticDataClass, FTransform Transform, AActor* Owner, APawn* Instigator)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (World && (World->IsNetMode(NM_DedicatedServer) || World->IsNetMode(NM_ListenServer)))
	{
		AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(), Transform, Owner, Instigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (Projectile)
		{
			Projectile->ProjectileStaticDataClass = ProjectileStaticDataClass;
			Projectile->FinishSpawning(Transform);
			return Projectile;
		}
	}
	return nullptr;
}
