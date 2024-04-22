// Fill out your copyright notice in the Description page of Project Settings.


#include "Tournament/InjectorHolder.h"

#include "PickupActor.h"


// Sets default values
AInjectorHolder::AInjectorHolder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInjectorHolder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInjectorHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInjectorHolder::GenerateRandomInjectors(int Count)
{
	if(!GetWorld())
	{
		return;
	}
	
	int MaxRandCount = TypesOfInjector.Num();
	for(int i = 0; i < Count; i++)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		
		int32 RandInjectorIndex = FMath::RandRange(0, MaxRandCount - 1);
		APickupActor* Injector = GetWorld()->SpawnActor<APickupActor>(TypesOfInjector[RandInjectorIndex], FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
	}
}


