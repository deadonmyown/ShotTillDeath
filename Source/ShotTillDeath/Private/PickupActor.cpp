#include "PickupActor.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionComponent.h"
#include "ItemInterface.h"
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

bool APickupActor::ActivatePickup(AShotTillDeathCharacter* OtherActor)
{
	if (!IsValid(OtherActor))
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

bool APickupActor::FinishInteraction_Implementation(AActor* OtherActor)
{
	if(AShotTillDeathCharacter* OtherCharacter = Cast<AShotTillDeathCharacter>(OtherActor))
	{
		return ActivatePickup(OtherCharacter);
	}
	return false;
}

void APickupActor::AttachItem()
{
	//TargetActor = TargetCharacter;

	// Check that the character is valid, and has no item yet
	if (!IsValid(TargetActor) || TargetActor->GetHasItem())
	{
		return;
	}

	// Attach the item to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(TargetActor->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// switch bHasItem so the animation blueprint can switch to another animation set
	TargetActor->SetHasItem(true);

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
			EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &APickupActor::TryUseItem);
			EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &APickupActor::DropItem);
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

	TargetActor->SetHasItem(false);

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
			EnhancedInputComponent->ClearActionBindings();
		}
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
	if (!TargetActor || !TargetActor->GetController() || !HasItemInterface())
	{
		return;
	}

	bool bIsSuccess = Execute_UseItem(this);
	OnUseItem.Broadcast(bIsSuccess);
}

bool APickupActor::HasItemInterface()
{
	return GetClass()->ImplementsInterface(UItemInterface::StaticClass());
}

void APickupActor::SetCurrentTransformByDefault()
{
	DefaultTransform = GetTransform();
}


