#include "GameSingleton.h"
#include "Events/GlobalEventHandler.h"

// Sets default values
AGameSingleton::AGameSingleton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameSingleton::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameSingleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UGlobalEventHandler * AGameSingleton::GetEventHandler()
{
	AGameSingleton* singleton = Cast<AGameSingleton>(GEngine->GameSingleton);
	if (singleton)
	{
		if (!singleton->mEventHandler)
		{
			singleton->mEventHandler = NewObject<UGlobalEventHandler>();
		}
		return singleton->mEventHandler;
	}
	return nullptr;
}