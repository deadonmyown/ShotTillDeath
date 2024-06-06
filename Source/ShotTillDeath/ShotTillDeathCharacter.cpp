#include "ShotTillDeathCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InteractionQueueComponent.h"
#include "Engine/LocalPlayer.h"
#include "Tournament/TournamentManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AShotTillDeathCharacter::AShotTillDeathCharacter()
{
	bCanTakeItem = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	//Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	//Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//Create Interaction Queue Component
	InteractionQueueComponent=CreateDefaultSubobject<UInteractionQueueComponent>(TEXT("InteractionQueueComponent"));

}

void AShotTillDeathCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MovementMappingContext, 0);
			Subsystem->AddMappingContext(DefaultMappingContext, 1);
			Subsystem->AddMappingContext(ItemMappingContext, 2);
		}
	}

}

void AShotTillDeathCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AShotTillDeathCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AShotTillDeathCharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShotTillDeathCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShotTillDeathCharacter::Look);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AShotTillDeathCharacter::Interact);
		
		EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Triggered, this, &AShotTillDeathCharacter::ExitTournament);
		
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &AShotTillDeathCharacter::TryUseItem);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AShotTillDeathCharacter::DropItem);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AShotTillDeathCharacter::EndTournamentSetupInputs_Implementation()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(TournamentMappingContext);
			Subsystem->AddMappingContext(MovementMappingContext, 0);
		}
	}
}

void AShotTillDeathCharacter::StartTournamentClearInputs_Implementation()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(TournamentMappingContext, 3);
			Subsystem->RemoveMappingContext(MovementMappingContext);
		}
	}
}

void AShotTillDeathCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AShotTillDeathCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AShotTillDeathCharacter::Interact()
{
	if(!Controller || !InteractionQueueComponent)
	{
		return;
	}

	InteractionQueueComponent->StartInteraction();
}

void AShotTillDeathCharacter::Jump()
{
	Super::Jump();
}

void AShotTillDeathCharacter::StopJumping()
{
	Super::StopJumping();
}
