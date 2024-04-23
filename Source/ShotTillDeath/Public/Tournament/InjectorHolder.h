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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InjectorHolder")
	APickupActor* CurrentActiveInjector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InjectorHolder")
	TArray<FVector> CurrentInjectorLocations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InjectorHolder")
	FVector FirstLocationForInjectors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InjectorHolder")
	FVector LastLocationForInjectors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InjectorHolder")
	FVector ActiveLocationForInjectors;
	
	UFUNCTION(BlueprintCallable, Category = "InjectorHolder")
	void GenerateRandomInjectors(int Count);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="InjectorHolder")
	void SetFirstPointForInjectors();

	UFUNCTION(BlueprintCallable, CallInEditor, Category="InjectorHolder")
	void SetLastPointForInjectors();

	UFUNCTION(BlueprintCallable, CallInEditor, Category="InjectorHolder")
	void SetActiveLocationForInjectors();

	UFUNCTION(BlueprintCallable, Category = "InjectorHolder")
	void SetInjectorsLocations(int Count);

	UFUNCTION(BlueprintCallable, Category = "InjectorHolder")
	void SetNewActiveInjectorOnUseItem(APickupActor* PickupActor, bool IsUseItemSuccess);

	UFUNCTION(BlueprintCallable, Category = "InjectorHolder")
	void SetNewActiveInjector();

	UFUNCTION(BlueprintCallable, Category = "InjectorHolder")
	void ResetInjectors();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> FirstLocationComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> LastLocationComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> ActiveLocationComponent = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> InjectorRootComponent = nullptr;
};
