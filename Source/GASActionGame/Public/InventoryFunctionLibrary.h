// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryFunctionLibrary.generated.h"

class AProjectile;
class UProjectileStaticData;
class UGameplayEffect;
class UItemStaticData;

UCLASS()
class GASACTIONGAME_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UItemStaticData* GetItemStaticData(const TSubclassOf<UItemStaticData> ItemDataClass);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm="IgnoreActors"))
	static void ApplyRadialDamage(const UObject* WorldContextObject, AActor* DamageCauser, FVector Location, float DamageRadius, float DamageAmount, TArray<TSubclassOf<UGameplayEffect>> DamageEffects, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, TEnumAsByte<ETraceTypeQuery> TraceType);

	UFUNCTION(BlueprintCallable, Meta = (WorldContext = "WorldContextObject"))
	static AProjectile* LaunchProjectile(const UObject* WorldContextObject, TSubclassOf<UProjectileStaticData> ProjectileStaticDataClass, FTransform Transform, AActor* Owner, APawn* Instigator);
};
