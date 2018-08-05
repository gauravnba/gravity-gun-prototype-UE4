#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GlobalEventHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGravityGunEvent_OnObjectDetected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGravityGunEvent_OnNoObjectDetected);

UCLASS()
class GRAVITYGUNTEST_API UGlobalEventHandler : public UObject
{
	GENERATED_BODY()

public:
	UGlobalEventHandler();
	~UGlobalEventHandler();

	UPROPERTY(BlueprintAssignable, Category = "Gravity Gun Events")
	FGravityGunEvent_OnObjectDetected OnObjectDetected;

	UPROPERTY(BlueprintAssignable, Category = "Gravity Gun Events")
	FGravityGunEvent_OnNoObjectDetected OnNoObjectDetected;
};
