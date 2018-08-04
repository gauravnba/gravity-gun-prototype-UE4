// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

/***
	Parent class for all weapons in the game. This class is designed to be used as an Abstract Class, but has no pure virtual methods since we have derived it from AActor.
*/
UCLASS()
class GRAVITYGUNTEST_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	*	Implementable method for the primary fire functionality of the weapon.
	*	Typically called when the user presses the left click.
	*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire() PURE_VIRTUAL(AWeapon::Fire,);

	/**
	*	Implementable method for the secondary fire functionality of the weapon or used to aim down iron-sights.
	*	This action is typically called with the right click.
	*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void SecondaryFire() PURE_VIRTUAL(AWeapon::SecondaryFire,);

	UPROPERTY(BlueprintReadWrite, Category="Weapon", meta = (DisplayName = "Weapon Mesh"))
	USkeletalMeshComponent* mMesh;		/**< Reference to the mesh of the gun. Currently being set by the derived blueprint class. */
};
