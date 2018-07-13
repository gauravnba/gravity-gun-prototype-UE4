#include "FXManager.h"
#include "GameSingleton/GameSingleton.h"
#include "Events/GlobalEventHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/GravityGun.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AFXManager::AFXManager() :
	mActiveParticle(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFXManager::BeginPlay()
{
	Super::BeginPlay();
	UGlobalEventHandler* eventHandler = AGameSingleton::GetEventHandler();

	if (eventHandler)
	{
		eventHandler->OnLaunchObject.AddDynamic(this, &AFXManager::OnGravityGunLaunchObject);
		eventHandler->OnGravitizeObject.AddDynamic(this, &AFXManager::OnGravityGunGravitizeObject);
		eventHandler->OnDropObject.AddDynamic(this, &AFXManager::OnGravityGunDropObject);
	}
}

// Called every frame
void AFXManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFXManager::OnGravityGunLaunchObject(AGravityGun* gravityGun)
{
	FTransform particleTransform = (gravityGun->mMesh)->GetSocketTransform("Ammo");
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mGravityGunLaunchObjectParticle, particleTransform.GetLocation(), particleTransform.Rotator(), particleTransform.GetScale3D());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), mGravityGunLaunchObjectSound, particleTransform.GetLocation());
}

void AFXManager::OnGravityGunGravitizeObject(AGravityGun* gravityGun)
{
	FTransform particleTransform = (gravityGun->mMesh)->GetSocketTransform("Ammo");
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), mGravityGunGravitizeObjectSound, particleTransform.GetLocation());

	mActiveParticle = UGameplayStatics::SpawnEmitterAttached(mGravityGunActiveParticle, gravityGun->mMesh, TEXT("Ammo"), particleTransform.GetLocation(), particleTransform.Rotator(), EAttachLocation::KeepWorldPosition, false);
}

void AFXManager::OnGravityGunDropObject(AGravityGun * gravityGun)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Called Drop Object"));
	mActiveParticle->DestroyComponent();
}
