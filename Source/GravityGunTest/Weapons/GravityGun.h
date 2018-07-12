// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GravityGun.generated.h"

UCLASS()
class GRAVITYGUNTEST_API AGravityGun : public AWeapon
{
	GENERATED_BODY()
	
public:
	AGravityGun();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/**
		Launches objects with a powerful beam. Works on both picked up objects as well as ones on the ground.
	*/
	UFUNCTION(BlueprintCallable, Category="Gravity Gun")
	virtual void Fire() override;

	/**
		Pulls the object that is in the cross-hairs to the weapon. If an object is already attached, drops it.
	*/
	UFUNCTION(BlueprintCallable, Category = "Gravity Gun")
	virtual void SecondaryFire() override;

private:
	UFUNCTION()
	inline bool DetectObject();

	UFUNCTION()
	inline void DropAttachedObject();

	UPROPERTY()
	bool mIsGravityActive;							/*< Boolean indicates whether an object is attached the gravity gun or not. */

	UPROPERTY()
	USceneComponent* mGravitizedObject;				/*< Reference to the scene component of the object attached to the gun. */

	static const float LAUNCH_IMPULSE_MAGNITUDE;	/*< The magnitude of the impulse that launches the gravitized object away from the gun. */
};