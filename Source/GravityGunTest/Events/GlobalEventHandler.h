#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GlobalEventHandler.generated.h"

class AGravityGun;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGravityGunEvent_OnLaunchObject, AGravityGun*, gravityGun);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGravityGunEvent_OnGravitizeObject, AGravityGun*, gravityGun);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGravityGunEvent_OnDropObject, AGravityGun*, gravityGun);

/**
 * 
 */
UCLASS()
class GRAVITYGUNTEST_API UGlobalEventHandler : public UObject
{
	GENERATED_BODY()

public:
	UGlobalEventHandler();
	~UGlobalEventHandler();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gravity Gun Events")
	FGravityGunEvent_OnLaunchObject OnLaunchObject;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gravity Gun Events")
	FGravityGunEvent_OnGravitizeObject OnGravitizeObject;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gravity Gun Events")
	FGravityGunEvent_OnDropObject OnDropObject;
};
