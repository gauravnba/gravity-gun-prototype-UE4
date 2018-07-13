#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FXManager.generated.h"

UCLASS()
class GRAVITYGUNTEST_API AFXManager : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AFXManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Called when OnLaunchObject event is fired. It plays the sound and particle effects corresponding to firing the object.
	*/
	UFUNCTION()
	void OnGravityGunLaunchObject(class AGravityGun* gravityGun);

	/**
	* Called when an object is gravitized (attached) by the gravity gun. Plays the sound and the particle effects.
	*/
	UFUNCTION()
	void OnGravityGunGravitizeObject(class AGravityGun* gravityGun);

	/**
	* Called when an object is dropped (attached) by the gravity gun. Stops the sound and particles playing while object was gravitized.
	*/
	UFUNCTION()
	void OnGravityGunDropObject(class AGravityGun* gravityGun);

private:
	UPROPERTY(EditAnywhere, Category = "Particle Sources", meta=(DisplayName="Launch Object Particle Effect"))
	class UParticleSystem* mGravityGunLaunchObjectParticle;

	UPROPERTY(EditAnywhere, Category = "Particle Sources", meta = (DisplayName = "Gravity Active Particle Effect"))
	class UParticleSystem* mGravityGunActiveParticle;

	UPROPERTY(EditAnywhere, Category = "Audio Sources", meta = (DisplayName = "Launch Object Audio"))
	class USoundBase* mGravityGunLaunchObjectSound;

	UPROPERTY(EditAnywhere, Category = "Audio Sources", meta = (DisplayName = "Gravitize Object Audio"))
	class USoundBase* mGravityGunGravitizeObjectSound;

	class UParticleSystemComponent* mActiveParticle;
};