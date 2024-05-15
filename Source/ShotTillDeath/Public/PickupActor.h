﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "InteractionQueueComponent.h"
#include "ItemInterface.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

class AShotTillDeathBaseCharacter;
class UInteractionComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickupActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUseItem, APickupActor*, CurrPickupActor, bool, bIsSuccess);

UCLASS(Blueprintable, BlueprintType)
class SHOTTILLDEATH_API APickupActor : public AActor, public IInteractionInterface, public IItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupActor();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category="Item")
	FOnUseItem OnUseItem;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* ItemMappingContext;

	/** Use Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* UseAction;

	/** Drop Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DropAction;

	UFUNCTION(BlueprintCallable, Category="Pickup")
	bool TakePickupItem(AShotTillDeathBaseCharacter* OtherActor);

	UFUNCTION(BlueprintCallable, Category="Item")
	void AttachItem();

	UFUNCTION(BlueprintCallable, Category="Item")
	void DetachItem();

	UFUNCTION(BlueprintCallable, Category="Item")
	void DropItem();

	UFUNCTION(BlueprintCallable, Category="Item")
	void TryUseItem();

	UFUNCTION(BlueprintCallable, Category="Item")
	bool HasItemInterface();

	UFUNCTION(BlueprintGetter, Category="Pickup")
	FInteractionData GetInteractionData() const;

	UFUNCTION(BlueprintSetter, Category="Pickup")
	void SetInteractionData(const FInteractionData& Value);

	UFUNCTION(BlueprintCallable, Category="Pickup")
	void SetReturnOnDefaultLocation(bool NewValue);

	UFUNCTION(BlueprintCallable, Category="Pickup")
	void SetDefaultLocation(FVector NewLocation);
	
	/**
	 * Called when the pickup effect was successfully activated;
	 */
	UPROPERTY(BlueprintAssignable, Category="Pickup")
	FOnPickupActivated OnPickupActivated;

	/**
	 * Activates the pickup logic.
	 * Call this function if you want to activate the pickup in your custom pickup class.
	 */
	UFUNCTION(BlueprintCallable, Category="Pickup")
	bool ActivatePickup(AShotTillDeathBaseCharacter* OtherActor);
	
	/**
	 * Enables the pickup if it was disabled.
	 * Works if DestroyOnActivation == false.
	 */
	UFUNCTION(BlueprintCallable, Category="Pickup")
	virtual void EnablePickup();

	UFUNCTION(BlueprintCallable, Category="Pickup")
	virtual void DisablePickup();
protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UInteractionComponent> InteractionTriggerComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetInteractionData, BlueprintSetter=SetInteractionData,
		Category="Pickup")
	FInteractionData InteractionData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> PickupRootComponent = nullptr;

	/**
	 * If true the pickup actor will destroy on activation, else it'll be disabled and hidden in game.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pickup")
	bool bDestroyOnActivation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	FTransform DefaultTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	bool bReturnOnDefaultLocation = false;

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Pickup")
	void SetCurrentTransformByDefault();

	/**
	 * Called when the pickup was enabled.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void OnPickupEnabled();

	/**
	 * Called when the pickup wan disabled.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void OnPickupDisabled();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess))
	AShotTillDeathBaseCharacter* TargetActor = nullptr;

	virtual bool FinishInteraction_Implementation(AActor* OtherActor) override;

	struct FEnhancedInputActionEventBinding* UseActionEventBinding = nullptr;

	FEnhancedInputActionEventBinding* DropActionEventBinding = nullptr;
};
