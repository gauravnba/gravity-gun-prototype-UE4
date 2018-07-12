// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityGun.h"
#include "Components/SkeletalMeshComponent.h"

#include "DrawDebugHelpers.h"
#include "EngineGlobals.h"

const float AGravityGun::LAUNCH_IMPULSE_MAGNITUDE = 2400.0f;

// Sets default values
AGravityGun::AGravityGun() :
	mIsGravityActive(false), mGravitizedObject(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGravityGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGravityGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGravityGun::SecondaryFire()
{
	if (!mIsGravityActive)
	{
		FVector start = mMesh->GetSocketLocation("Ammo");
		FVector end = start + (1000 * mMesh->GetRightVector());

#if UE_BUILD_DEVELOPMENT
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2, 0, 1.0f);
#endif

		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("gravityGunTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bTraceAsyncScene = true;
		traceParams.bReturnPhysicalMaterial = false;

		//Re-initialize hit info
		FHitResult hitResult(ForceInit);

		//call GetWorld() from within an actor extending class
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			USceneComponent* hitComponent = hitResult.Actor.Get()->GetRootComponent();
			if (hitComponent->IsSimulatingPhysics())
			{
#if UE_BUILD_DEVELOPMENT
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, GetDebugName(hitResult.Actor.Get()));
#endif
				mGravitizedObject->AttachToComponent(mMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), "GravityFocus");
				Cast<UPrimitiveComponent>(mGravitizedObject)->SetSimulatePhysics(false);
				mIsGravityActive = true;
			}
		}
	}

	else
	{
		mGravitizedObject->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		Cast<UPrimitiveComponent>(mGravitizedObject)->SetSimulatePhysics(true);
		mIsGravityActive = false;
		mGravitizedObject = nullptr;
	}
}

inline bool AGravityGun::DetectObject()
{
	if (!mIsGravityActive)
	{
		FVector start = mMesh->GetSocketLocation("Ammo");
		FVector end = start + (1000 * mMesh->GetRightVector());

#if UE_BUILD_DEVELOPMENT
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2, 0, 1.0f);
#endif

		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("gravityGunTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bTraceAsyncScene = true;
		traceParams.bReturnPhysicalMaterial = false;
		FHitResult hitResult(ForceInit);

		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			USceneComponent* hitComponent = hitResult.Actor.Get()->GetRootComponent();
			if (hitComponent->IsSimulatingPhysics())
			{
				mGravitizedObject = hitComponent;
				return true;
			}
		}
	}

	return false;
}

void AGravityGun::Fire()
{
	if (mIsGravityActive)
	{
		mGravitizedObject->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		Cast<UPrimitiveComponent>(mGravitizedObject)->SetSimulatePhysics(true);
		Cast<UPrimitiveComponent>(mGravitizedObject)->AddImpulse(LAUNCH_IMPULSE_MAGNITUDE * mMesh->GetRightVector(), NAME_None, true);
#if UE_BUILD_DEVELOPMENT
		DrawDebugLine(GetWorld(), mMesh->GetSocketLocation("Ammo"), (LAUNCH_IMPULSE_MAGNITUDE * mMesh->GetRightVector()), FColor::Red, false, 2, 0, 1.0f);
#endif
		mIsGravityActive = false;
		mGravitizedObject = nullptr;
	}

	else
	{
		FVector start = mMesh->GetSocketLocation("Ammo");
		FVector end = start + (1000 * mMesh->GetRightVector());

#if UE_BUILD_DEVELOPMENT
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2, 0, 1.0f);
#endif

		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("gravityGunTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bTraceAsyncScene = true;
		traceParams.bReturnPhysicalMaterial = false;

		//Re-initialize hit info
		FHitResult hitResult(ForceInit);

		//call GetWorld() from within an actor extending class
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			USceneComponent* hitComponent = hitResult.Actor.Get()->GetRootComponent();
			if (hitComponent->IsSimulatingPhysics())
			{
				Cast<UPrimitiveComponent>(hitComponent)->AddImpulse(LAUNCH_IMPULSE_MAGNITUDE * mMesh->GetRightVector(), NAME_None, true);	
			}
		}
	}
}