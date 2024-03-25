// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionQueueComponent.generated.h"

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	/**
	 * Toggles if the actor require being in the line of sight to be interacted. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	bool bRequireLineOfSight = false;

	/**
	 * A message which can be used in HUD.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	FString InteractionMessage = "Interact";

	/**
	 * How much time required to activate interaction effect.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData", meta=(ClampMin="0"))
	float InteractionTime = 0.f;
};

USTRUCT(BlueprintType)
struct FQueueData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QueueData")
	AActor* Actor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QueueData")
	FInteractionData InteractionData;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteraction, AActor*, TargetActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQueueChanged, const AActor*, OtherActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOTTILLDEATH_API UInteractionQueueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionQueueComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/** Called when the interaction process started. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteraction OnInteractionStarted;

	/** Called when the interaction effect successfully activated. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteraction OnInteractionFinished;

	/** Called when the interaction process stopped. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteraction OnInteractionStopped;

	/** Called when actor was added to the queue. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnQueueChanged OnActorAdded;

	/** Called when actor was added to the queue. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnQueueChanged OnActorRemoved;

	/** Adds interaction data into the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool Add(AActor* Actor, const FInteractionData& InteractionData);

	/** Removes interaction data into the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool Remove(const AActor* Actor);

	/** Starts interaction with the first actor in the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool StartInteraction();

	/** Finishes interaction.
	 * Call it if FinishManually == false.
	 */
	UFUNCTION()
	bool FinishInteraction(AActor* Actor);

	/** Stops interaction.*/
	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool StopInteraction();

private:
	/** If true, the FinishInteraction() must be called manually.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	bool bFinishManually = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	TArray<FQueueData> InteractionQueue;
		
};
