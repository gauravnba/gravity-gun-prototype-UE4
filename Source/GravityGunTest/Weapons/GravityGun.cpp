#include "GravityGun.h"
#include "Components/SkeletalMeshComponent.h"

#include "DrawDebugHelpers.h"
#include "EngineGlobals.h"
#include "GameSingleton/GameSingleton.h"
#include "Events/GlobalEventHandler.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

AGravityGun::AGravityGun() :
	mIsGravityActive(false), mGravitizedObject(nullptr), mDetectedFlag(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mPhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("mPhysicsHandle"));
}

void AGravityGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGravityGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set the object's rotation and location every tick.
	if (mIsGravityActive)
	{
		mPhysicsHandle->SetTargetLocationAndRotation(mMesh->GetSocketLocation("GravityFocus"), mMesh->GetComponentRotation());
	}

	// Check if an object is detected and send relevant events to update the crosshair.
	if (!mDetectedFlag && DetectObject())
	{
		AGameSingleton::GetEventHandler()->OnObjectDetected.Broadcast();
		mDetectedFlag = true;
	}
	else if (mDetectedFlag && !DetectObject())
	{
		AGameSingleton::GetEventHandler()->OnNoObjectDetected.Broadcast();
		mDetectedFlag = false;
	}
}

void AGravityGun::Fire()
{
	// If the gravity is active (object is attached), drop it. Else detect if an object is in range.
	if (mIsGravityActive)
	{
		DropObject();
	}

	// Apply Impulse to captured object.
	if (mGravitizedObject)
	{
		Cast<UPrimitiveComponent>(mGravitizedObject)->AddImpulse(LAUNCH_IMPULSE_MAGNITUDE * mMesh->GetRightVector(), NAME_None, true);
		mGravitizedObject = nullptr;
	}

	// Spawn particle effect and sound at location.
	FTransform particleTransform = mMesh->GetSocketTransform("Ammo");
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mGravityGunLaunchObjectParticle, particleTransform.GetLocation(), particleTransform.Rotator(), particleTransform.GetScale3D());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), mGravityGunLaunchObjectSound, particleTransform.GetLocation());
}

void AGravityGun::SecondaryFire()
{
	// If an object is attached, drop it. Else, detect an object in range and grab onto it.
	if (mIsGravityActive)
	{
		DropObject();
		mGravitizedObject = nullptr;
	}
	else
	{
		if (mGravitizedObject)
		{
			// Attach the object keeping the world transform of the object and shut down physics on it.
			mPhysicsHandle->GrabComponentAtLocationWithRotation(Cast<UPrimitiveComponent>(mGravitizedObject), TEXT(""), mGravitizedObject->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
			mIsGravityActive = true;

			// Spawn particle effect and sound at location.
			FTransform particleTransform = mMesh->GetSocketTransform("Ammo");
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), mGravityGunGravitizeObjectSound, particleTransform.GetLocation());
			mActiveParticle = UGameplayStatics::SpawnEmitterAttached(mGravityGunActiveParticle, mMesh, TEXT("Ammo"), particleTransform.GetLocation(), particleTransform.Rotator(), EAttachLocation::KeepWorldPosition, false);
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

		if (mGravitizedObject)
		{
			mGravitizedObject = false;
		}
	}

	return false;
}

void AGravityGun::DropObject()
{
	if (mGravitizedObject && mIsGravityActive)
	{
		// Detach the object preserving the current world transform and turn the physics back on.
		mPhysicsHandle->ReleaseComponent();
		mIsGravityActive = false;

		// Spawn particle effect and sound at location.
		mActiveParticle->DestroyComponent();
	}
}