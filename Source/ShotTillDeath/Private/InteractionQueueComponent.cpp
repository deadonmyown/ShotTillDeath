#include "InteractionQueueComponent.h"

UInteractionQueueComponent::UInteractionQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UInteractionQueueComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UInteractionQueueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UInteractionQueueComponent::Add(AActor* Actor, const FInteractionData& InteractionData)
{
}

bool UInteractionQueueComponent::Remove(const AActor* Actor)
{
}

bool UInteractionQueueComponent::StartInteraction()
{
}

bool UInteractionQueueComponent::FinishInteraction(AActor* Actor)
{
}

bool UInteractionQueueComponent::StopInteraction()
{
}

