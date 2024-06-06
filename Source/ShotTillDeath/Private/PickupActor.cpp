﻿#include "PickupActor.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionComponent.h"
#include "ItemInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ShotTillDeath/ShotTillDeathCharacter.h"

APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupRootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(ToRawPtr(PickupRootComponent));

	InteractionTriggerComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionTrigger");
	InteractionTriggerComponent->SetupAttachment(GetRootComponent());
	UInteractionComponent::SetTriggerDefaultCollision(InteractionTriggerComponent);
}

void APickupActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (InteractionTriggerComponent)
	{
		InteractionTriggerComponent->SetInteractionData(InteractionData);
	}
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	InteractionTriggerComponent->SetInteractionData(InteractionData);
}

void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APickupActor::ActivatePickup(AShotTillDeathBaseCharacter* OtherActor)
{
	if (!IsValid(OtherActor) || !OtherActor->CanTakeItem())
	{
		return false;
	}

	TargetActor = OtherActor;
	
	OnPickupActivated.Broadcast();

	if (bDestroyOnActivation)
	{
		Destroy();
	}
	else
	{
		DisablePickup();
		AttachItem();
	}

	return true;
}

bool APickupActor::TakePickupItem(AShotTillDeathBaseCharacter* OtherActor)
{
	if (!IsValid(OtherActor) || !OtherActor->CanTakeItem())
	{
		return false;
	}

	TargetActor = OtherActor;
	
	OnPickupActivated.Broadcast();

	if (bDestroyOnActivation)
	{
		Destroy();
	}
	else
	{
		DisablePickup();
		AttachItem();
	}

	return true;
}

void APickupActor::EnablePickup()
{
	/*if (!IsHidden())
	{
		return;
	}*/

	//SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	OnPickupEnabled();
}

void APickupActor::DisablePickup()
{
	/*if (IsHidden())
	{
		return;
	}*/

	//SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	OnPickupDisabled();
}

FInteractionData APickupActor::GetInteractionData() const
{
	return InteractionData;
}

void APickupActor::SetInteractionData(const FInteractionData& Value)
{
	InteractionData = Value;
}

void APickupActor::SetReturnOnDefaultLocation(bool NewValue)
{
	bReturnOnDefaultLocation = NewValue;
}

void APickupActor::SetDefaultTransform(FTransform NewTransform)
{
	DefaultTransform = NewTransform;
}

bool APickupActor::FinishInteraction_Implementation(AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return false;
	}
	
	if(AShotTillDeathBaseCharacter* OtherCharacter = Cast<AShotTillDeathBaseCharacter>(OtherActor))
	{
		return ActivatePickup(OtherCharacter);
	}
	return false;
}

void APickupActor::AttachItem()
{
	// Check that the character is valid, and has no item yet
	if (!IsValid(TargetActor) || !TargetActor->CanTakeItem())
	{
		return;
	}
	
	// Attach the item to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(TargetActor->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	SetActorRelativeTransform(PickupTransform);
	
	// switch bHasItem so the animation blueprint can switch to another animation set
	TargetActor->SetItem(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(TargetActor->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ItemMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// BindAction
			UseActionEventBinding = &EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &APickupActor::TryUseItem);
			DropActionEventBinding = &EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &APickupActor::DropItem);
			
		}
	}
}

void APickupActor::DetachItem()
{
	if(!IsValid(TargetActor) || !TargetActor->GetHasItem())
	{
		return;
	}
	
	const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachmentRules);
	SetActorRotation(FRotator::ZeroRotator);

	TargetActor->ResetItem();

	//Clear inputs
	if (APlayerController* PlayerController = Cast<APlayerController>(TargetActor->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(ItemMappingContext);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// UnbindAction
			EnhancedInputComponent->RemoveBinding(*UseActionEventBinding);
			EnhancedInputComponent->RemoveBinding(*DropActionEventBinding);
		}
	}

	
	FHitResult HitResult = GetFloor();
	if(IsValid(HitResult.GetActor()))
	{
		UE_LOG(LogTemp, Display, TEXT("Trace Floor: %s"), *HitResult.GetActor()->GetFName().ToString());
		SetActorLocation(HitResult.Location);
	}
	
	TargetActor = nullptr;
}

void APickupActor::DropItem()
{
	if (!TargetActor || !TargetActor->GetController())
	{
		return;
	}

	EnablePickup();
	DetachItem();
	if(bReturnOnDefaultLocation)
	{
		SetActorTransform(DefaultTransform);
	}
}

void APickupActor::TryUseItem()
{
	if (!TargetActor || !TargetActor->GetController() || !HasItemInterface() || !bCanUse)
	{
		return;
	}

	bool bIsSuccess = Execute_UseItem(this);
	OnUseItem.Broadcast(this, bIsSuccess);
}

bool APickupActor::HasItemInterface()
{
	return GetClass()->ImplementsInterface(UItemInterface::StaticClass());
}

void APickupActor::SetCurrentTransformByDefault()
{
	DefaultTransform = GetTransform();
}

void APickupActor::SetCurrentTransformByPickup()
{
	PickupTransform = GetTransform();
}


FHitResult APickupActor::GetFloor()
{
	if(!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Display, TEXT("No Target Actor"));
		return FHitResult();
	}
	
	FVector ViewLocation{GetActorLocation()};
	FRotator ViewRotation{FRotator::ZeroRotator};

	const FVector TraceStart {ViewLocation};
	const FVector TraceDirection {FVector(0,0,-1)};
	const FVector TraceEnd {TraceStart + TraceDirection * SightDistance};

	if(!GetWorld())
	{
		UE_LOG(LogTemp, Display, TEXT("No World"));
		return FHitResult();
	}

	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		TraceStart,
		TraceEnd,
		TraceChannel,
		false,
		{GetOwner(), TargetActor},
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		DebugDrawTime);
	
	return HitResult;
}


