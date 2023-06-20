// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/Projectile.h"
#include "AGTypes.h"
#include "InventoryFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

TAutoConsoleVariable<bool> CVarShowDebugProjectile(TEXT("AG.ShowDebugProjectile"), false, TEXT("Show information about projectile in output log."), ECVF_Cheat);

AProjectile::AProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &ThisClass::OnProjectileStop);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetIsReplicated(true);
	StaticMeshComponent->SetCollisionProfileName("Projectile");
	StaticMeshComponent->SetReceivesDecals(false);
	StaticMeshComponent->SetupAttachment(GetRootComponent());
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	SetReplicates(true);

	if (const UProjectileStaticData* ProjectileData = GetProjectileStaticData())
	{
		if (ProjectileData->StaticMesh && StaticMeshComponent)
		{
			StaticMeshComponent->SetStaticMesh(ProjectileData->StaticMesh);
		}

		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
			ProjectileMovementComponent->InitialSpeed = ProjectileData->InitialSpeed;
			ProjectileMovementComponent->MaxSpeed = ProjectileData->MaxSpeed;
			ProjectileMovementComponent->bRotationFollowsVelocity = true;
			ProjectileMovementComponent->bShouldBounce = false;
			ProjectileMovementComponent->Bounciness = 0;
			ProjectileMovementComponent->ProjectileGravityScale = ProjectileData->GravityMultiplayer;
			ProjectileMovementComponent->Velocity = ProjectileData->InitialSpeed * GetActorForwardVector();
		}
	}

	if (CVarShowDebugProjectile.GetValueOnAnyThread())
	{
		DebugDrawPath();
	}
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UProjectileStaticData* ProjectileData = GetProjectileStaticData())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileData->ImpactSFX, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileData->ImpactVFX, GetActorLocation());
	}
	Super::EndPlay(EndPlayReason);
}

void AProjectile::DebugDrawPath()
{
	if (const UProjectileStaticData* ProjectileData = GetProjectileStaticData())
	{
		FPredictProjectilePathParams PredictProjectilePathParams;
		PredictProjectilePathParams.StartLocation = GetActorLocation();
		PredictProjectilePathParams.LaunchVelocity = ProjectileData->InitialSpeed * GetActorLocation();
		PredictProjectilePathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
		PredictProjectilePathParams.bTraceComplex = true;
		PredictProjectilePathParams.bTraceWithCollision = true;
		PredictProjectilePathParams.DrawDebugTime = 3.f;
		PredictProjectilePathParams.DrawDebugType = EDrawDebugTrace::ForDuration;
		PredictProjectilePathParams.OverrideGravityZ = ProjectileData->GravityMultiplayer == 0 ? 0.0001f : ProjectileData->GravityMultiplayer;

		FPredictProjectilePathResult PredictProjectilePathResult;
		if (UGameplayStatics::PredictProjectilePath(this, PredictProjectilePathParams, PredictProjectilePathResult))
		{
			DrawDebugSphere(GetWorld(), PredictProjectilePathResult.HitResult.Location, 50.f, 10, FColor::Red);
		}
	}
}

void AProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	if (const UProjectileStaticData* ProjectileData = GetProjectileStaticData())
	{
		UInventoryFunctionLibrary::ApplyRadialDamage(this, GetOwner(), GetActorLocation(), ProjectileData->DamageRadius, ProjectileData->BaseDamage, ProjectileData->Effects, ProjectileData->RadialDamageQueryTypes, ProjectileData->RadialDamageTraceType);
	}
	Destroy();
}

const UProjectileStaticData* AProjectile::GetProjectileStaticData() const
{
	if (IsValid(ProjectileStaticDataClass))
	{
		return GetDefault<UProjectileStaticData>(ProjectileStaticDataClass);
	}
	return nullptr;
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ProjectileStaticDataClass);
}