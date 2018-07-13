#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSingleton.generated.h"

class UGlobalEventHandler;

UCLASS()
class GRAVITYGUNTEST_API AGameSingleton : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGameSingleton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Global accessor method for the event handler.
	*/
	UFUNCTION(BlueprintPure, Category = "Global Event Handler")
	static UGlobalEventHandler* GetEventHandler();

private:
	UPROPERTY(EditAnywhere, Category = "Global Event Handler")
	UGlobalEventHandler* mEventHandler;		/**< The globally accessible event handler object. */
};
