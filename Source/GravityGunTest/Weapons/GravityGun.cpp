#include "GravityGun.h"
#include "Components/SkeletalMeshComponent.h"

#include "DrawDebugHelpers.h"
#include "EngineGlobals.h"
#include "GameSingleton/GameSingleton.h"
#include "Events/GlobalEventHandler.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

AGravityGun::AGravityGun() :
	mIsGravityActive(false), mGravitizedObject(nullptr), mDetectedFlag(false)
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
		DropAttachedObject();
	}
	//else
	//{
	//	DetectObject();
	//}

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

void AGravityGun::DropAttachedObject()
{
	if (mGravitizedObject && mIsGravityActive)
	{
		// Detach the object preserving the current world transform and turn the physics back on.
		mGravitizedObject->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		Cast<UPrimitiveComponent>(mGravitizedObject)->SetSimulatePhysics(true);
		mIsGravityActive = false;

		// Spawn particle effect and sound at location.
		mActiveParticle->DestroyComponent();
	}
}