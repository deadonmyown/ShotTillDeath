// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InjectorHolder.generated.h"

UCLASS()
class SHOTTILLDEATH_API AInjectorHolder : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInjectorHolder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InjectorHolder")
	TArray<TSubclassOf<class APickupActor>> TypesOfInjector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InjectorHolder")
	TArray<APickupActor*> CurrentInjectors;

	UFUNCTION(BlueprintCallable, Category = "InjectorHolder")
	void GenerateRandomInjectors(int Count);
};
