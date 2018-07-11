// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravityGun.generated.h"

UCLASS()
class GRAVITYGUNTEST_API AGravityGun : public AActor
{
	GENERATED_BODY()
	
public:
	AGravityGun();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/**
		Pulls the object that is in the cross-hairs to the weapon. If an object is already attached, drops it.
		Called when player presses the Pull/Drop button when this weapon is equipped.
	*/
	UFUNCTION(BlueprintCallable, Category="Gravity Gun")
	virtual void PullOrDrop();

	UFUNCTION(BlueprintCallable, Category = "Gravity Gun")
	virtual void Launch();

	UPROPERTY(BlueprintReadWrite, meta=(DisplayName="Weapon Mesh"))
	USkeletalMeshComponent* mMesh;					/*< Reference to the mesh of the gun. Currently being set in the derived blueprint class. */

private:
	UPROPERTY()
	bool mIsGravityActive;							/*< Boolean indicates whether an object is attached the gravity gun or not. */

	UPROPERTY()
	USceneComponent* mGravitizedObject;				/*< Reference to the scene component of the object attached to the gun. */

	static const float LAUNCH_IMPULSE_MAGNITUDE;	/*< The magnitude of the impulse that launches the gravitized object away from the gun. */
};