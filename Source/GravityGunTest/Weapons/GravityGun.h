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
	UFUNCTION(BlueprintCallable, Category="Gravity Gun")
	virtual void Fire() override;

	/**
	*	Pulls the object that is in the cross-hairs to the weapon. If an object is already attached, drops it.
	*/
	UFUNCTION(BlueprintCallable, Category = "Gravity Gun")
	virtual void SecondaryFire() override;

private:
	inline bool DetectObject();
	inline void DropAttachedObject();

	UPROPERTY()
	bool mIsGravityActive;							/**< Boolean indicates whether an object is attached the gravity gun or not. */

	UPROPERTY()
	USceneComponent* mGravitizedObject;				/**< Reference to the scene component of the object attached to the gun. */
													  
	static const float LAUNCH_IMPULSE_MAGNITUDE;	/**< The magnitude of the impulse that launches the gravitized object away from the gun. */
	static const float GRAVITY_GUN_RANGE;			/**< The range that the gun can operate under. */
	static const float LEVITATE_TO_LERP_ALPHA;		/**< Holds the alpha value to use when interpolating a gravitized object's location. */
};