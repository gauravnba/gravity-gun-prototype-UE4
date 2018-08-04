// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GravityGun.generated.h"

/**
	This class implements the functionality of the Gravity Gun, where it can pull a physics object to itself and also launch it.
	It is a derived class of AWeapon.
*/
UCLASS()
class GRAVITYGUNTEST_API AGravityGun : public AWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGravityGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	*	Launches objects with a powerful beam. Works on both picked up objects as well as ones on the ground.
	*/
	UFUNCTION(Category="Gravity Gun")
	virtual void Fire() override;

	/**
	*	Pulls the object that is in the cross-hairs to the weapon. If an object is already attached, drops it.
	*/
	UFUNCTION(Category = "Gravity Gun")
	virtual void SecondaryFire() override;

	/**
	*	Sends out a line trace to detect interactable objects.
	*	@return boolean true if a suitable Actor is in range (hit by the line trace).
	*/
	UFUNCTION(BlueprintCallable, Category = "Gravity Gun")
	bool DetectObject();

	/**
	*	Drops the attached object, if there is one. Also mutates the mIsGravityActive to false.
	*/
	UFUNCTION(BlueprintCallable, Category = "Gravity Gun")
	void DropAttachedObject();

private:

	UPROPERTY()
	bool mIsGravityActive;							/**< Boolean indicates whether an object is attached the gravity gun or not. */

	UPROPERTY()
	USceneComponent* mGravitizedObject;				/**< Reference to the scene component of the object attached to the gun. */
	
protected:
	UPROPERTY(EditAnywhere, Category = "Gravity Gun", meta = (DisplayName = "Launch Impulse Magnitude"))
	float LAUNCH_IMPULSE_MAGNITUDE;							/**< The magnitude of the impulse that launches the gravitized object away from the gun. */

	UPROPERTY(EditAnywhere, Category = "Gravity Gun", meta = (DisplayName = "Gravity Gun Range"))
	float GRAVITY_GUN_RANGE;								/**< The range that the gun can operate under. */

	UPROPERTY(EditAnywhere, Category = "Gravity Gun", meta=(DisplayName="Levitate Lerp Alpha"))
	float LEVITATE_TO_LERP_ALPHA;							/**< Holds the alpha value to use when interpolating a gravitized object's location. */

	UPROPERTY(EditAnywhere, Category = "Gravity Gun", meta = (DisplayName = "Launch Object Particle Effect"))
	class UParticleSystem* mGravityGunLaunchObjectParticle; /**< The particle effect reference to be played when object is launched. */

	UPROPERTY(EditAnywhere, Category = "Gravity Gun", meta = (DisplayName = "Gravity Active Particle Effect"))
	class UParticleSystem* mGravityGunActiveParticle;		/**< Particle reference to the effect when the gravity is active. */

	UPROPERTY(EditAnywhere, Category = "Gravity Gun", meta = (DisplayName = "Launch Object Audio"))
	class USoundBase* mGravityGunLaunchObjectSound;			/**< Sound to be played when object is launched. */

	UPROPERTY(EditAnywhere, Category = "Gravity Gun", meta = (DisplayName = "Gravitize Object Audio"))
	class USoundBase* mGravityGunGravitizeObjectSound;		/**< Sound played when object is grabbed. */

	UPROPERTY()
	class UParticleSystemComponent* mActiveParticle;		/**< Reference to the gravitize particle being played. */
};