#include "GravityGun.h"
#include "Components/SkeletalMeshComponent.h"

#include "DrawDebugHelpers.h"
#include "EngineGlobals.h"
#include "GameSingleton/GameSingleton.h"
#include "Events/GlobalEventHandler.h"

AGravityGun::AGravityGun() :
	mIsGravityActive(false), mGravitizedObject(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGravityGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGravityGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If an object is attached and is not at the focus of the gun, levitate it to the gun.
	if (mIsGravityActive && (mGravitizedObject->GetComponentLocation() != mMesh->GetSocketLocation("GravityFocus")))
	{
		mGravitizedObject->SetWorldLocation(FMath::Lerp(mGravitizedObject->GetComponentLocation(), mMesh->GetSocketLocation("GravityFocus"), LEVITATE_TO_LERP_ALPHA));
	}
}

void AGravityGun::Fire()
{
	// If the gravity is active (object is attached), drop it. Else detect if an object is in range.
	if (mIsGravityActive)
	{
		DropAttachedObject();
	}
	else
	{
		DetectObject();
	}

	// Apply Impulse to captured object.
	if (mGravitizedObject)
	{
		Cast<UPrimitiveComponent>(mGravitizedObject)->AddImpulse(LAUNCH_IMPULSE_MAGNITUDE * mMesh->GetRightVector(), NAME_None, true);
		mGravitizedObject = nullptr;
	}

	AGameSingleton::GetEventHandler()->OnLaunchObject.Broadcast(this);
}

void AGravityGun::SecondaryFire()
{
	// If an object is attached, drop it. Else, detect an object in range and grab onto it.
	if (mIsGravityActive)
	{
		DropAttachedObject();
		mGravitizedObject = nullptr;
	}
	else
	{
		if (DetectObject())
		{
			// Attach the object keeping the world transform of the object and shut down physics on it.
			mGravitizedObject->AttachToComponent(mMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), "GravityFocus");
			Cast<UPrimitiveComponent>(mGravitizedObject)->SetSimulatePhysics(false);

			mIsGravityActive = true;
			AGameSingleton::GetEventHandler()->OnGravitizeObject.Broadcast(this);
		}
	}
}

bool AGravityGun::DetectObject()
{
	if (!mIsGravityActive)
	{
		// Start and End points of the LineTrace
		FVector start = mMesh->GetSocketLocation("Ammo");					
		FVector end = start + (GRAVITY_GUN_RANGE * mMesh->GetRightVector());

		FCollisionQueryParams traceParams = FCollisionQueryParams("GravityGunTrace", true, this);
		FHitResult hitResult(ForceInit);

		// Check if an actor is hit. If the hit actor can simulate physics, assign it as the gravitized object and return true.
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

void AGravityGun::DropAttachedObject()
{
	if (mGravitizedObject)
	{
		// Detach the object preserving the current world transform and turn the physics back on.
		mGravitizedObject->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		Cast<UPrimitiveComponent>(mGravitizedObject)->SetSimulatePhysics(true);
		mIsGravityActive = false;
		AGameSingleton::GetEventHandler()->OnDropObject.Broadcast(this);
	}
}