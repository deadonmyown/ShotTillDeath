// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShopHolder.generated.h"

UCLASS()
class SHOTTILLDEATH_API AShopHolder : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShopHolder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShopHolder")
	TArray<FVector> LocationsForItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShopHolder")
	TArray<class APickupActor*> Items;
	
	UPROPERTY(VisibleAnywhere, Category="ShopHolder")
	int32 MaxItemsCount;

	UFUNCTION(BlueprintCallable, Category = "ShopHolder")
	void AddItem(APickupActor* Item);

	UFUNCTION(BlueprintCallable, Category = "ShopHolder")
	void RemoveItem(APickupActor* Item);
	
	UFUNCTION(BlueprintCallable, Category = "ShopHolder")
	void SetItemLocation(APickupActor* Item);

	UFUNCTION(BlueprintCallable, Category="ShopHolder")
	bool IsShopHolderFull();

	UFUNCTION(BlueprintCallable, Category = "ShopHolder")
	void OnUseItem(APickupActor* PickupActor, bool IsUseItemSuccess);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="ShopHolder")
	void SetLocationsForItems();
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category="ShopHolder")
	void ReinitializeComponents();

	UFUNCTION(BlueprintCallable, CallInEditor, Category="ShopHolder")
	void TryTakeRandomItem(AShotTillDeathBaseCharacter* OtherCharacter);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> ShopRootComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TArray<TObjectPtr<USceneComponent>> LocationsComponent;
};
