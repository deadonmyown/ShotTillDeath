#include "ItemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ItemInterface.h"
#include "ShotTillDeath/ShotTillDeathCharacter.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UItemComponent::AttachItem(AShotTillDeathCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no item yet
	if (Character == nullptr || Character->GetHasItem())
	{
		return;
	}

	// Attach the item to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	GetOwner()->AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// switch bHasItem so the animation blueprint can switch to another animation set
	Character->SetHasItem(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ItemMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// BindAction
			EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &UItemComponent::UseItem);
			EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &UItemComponent::DropItem);
		}
	}
}

void UItemComponent::DetachItem()
{
	if(Character == nullptr || !Character->GetHasItem())
	{
		return;
	}
	
	const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
	GetOwner()->DetachFromActor(DetachmentRules);

	Character->SetHasItem(false);

	//Clear inputs
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
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

	Character = nullptr;
}

void UItemComponent::DropItem()
{
	if (!Character || !Character->GetController())
	{
		return;
	}

	DetachItem();
}

void UItemComponent::UseItem()
{
	if (!Character || !Character->GetController() || !HasItemInterface())
	{
		return;
	}

	IItemInterface::Execute_UseItem(GetOwner());
	OnUseItem.Broadcast();
}

bool UItemComponent::HasItemInterface()
{
	return GetOwner()->GetClass()->ImplementsInterface(UItemInterface::StaticClass());
}

