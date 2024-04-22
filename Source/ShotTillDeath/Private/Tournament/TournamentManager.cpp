// Fill out your copyright notice in the Description page of Project Settings.


#include "Tournament/TournamentManager.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionComponent.h"
#include "ShotTillDeath/ShotTillDeathCharacter.h"


// Sets default values
ATournamentManager::ATournamentManager()
{
	PrimaryActorTick.bCanEverTick = true;

	TournamentRootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(ToRawPtr(TournamentRootComponent));

	InteractionTriggerComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionTrigger");
	InteractionTriggerComponent->SetupAttachment(GetRootComponent());
	UInteractionComponent::SetTriggerDefaultCollision(InteractionTriggerComponent);
}

void ATournamentManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (InteractionTriggerComponent)
	{
		InteractionTriggerComponent->SetInteractionData(InteractionData);
	}
}

// Called when the game starts or when spawned
void ATournamentManager::BeginPlay()
{
	Super::BeginPlay();

	InteractionTriggerComponent->SetInteractionData(InteractionData);
}

// Called every frame
void ATournamentManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATournamentManager::FinishInteraction_Implementation(AActor* OtherActor)
{
	if (!IsValid(OtherActor))
	{
		return false;
	}
	
	if(AShotTillDeathCharacter* OtherCharacter = Cast<AShotTillDeathCharacter>(OtherActor))
	{
		return StartTournament(OtherCharacter);
	}
	return false;
}

bool ATournamentManager::StartTournament(AShotTillDeathCharacter* OtherCharacter)
{
	if(!IsValid(OtherCharacter))
	{
		UE_LOG(LogTemp, Display, TEXT("Character is not valid"));
		return false;
	}
	if(OtherCharacter->bHasItem)
	{
		UE_LOG(LogTemp, Display, TEXT("Character hold item"));
		return false;
	}

	TournamentMainCharacter = OtherCharacter;

	TournamentMainCharacter->ClearPlayerInputs();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(TournamentMainCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(TournamentMappingContext, 2);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// BindAction
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, TournamentMainCharacter, &AShotTillDeathCharacter::Look);
			EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Triggered, this, &ATournamentManager::EndTournament);
		}
	}
	
	TournamentMainCharacter->SetActorLocation(MainCharacterPosition);
	TournamentMainCharacter->SetActorRotation(MainCharacterRotation);

	//TODO Create Enemy
	
	
	DisableInteraction();
	OnTournamentStart.Broadcast();
	
	return true;
}

void ATournamentManager::EndTournament()
{
	if(!IsValid(TournamentMainCharacter))
	{
		return;
	}
	
	EnableInteraction();

	if (APlayerController* PlayerController = Cast<APlayerController>(TournamentMainCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(TournamentMappingContext);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// UnbindAction
			EnhancedInputComponent->ClearActionBindings();
		}
	}

	TournamentMainCharacter->SetupPlayerInputs();
	
	OnTournamentEnd.Broadcast();
}


void ATournamentManager::EnableInteraction()
{
	SetActorEnableCollision(true);
}

void ATournamentManager::DisableInteraction()
{
	SetActorEnableCollision(false);
}

FInteractionData ATournamentManager::GetInteractionData() const
{
	return InteractionData;
}

void ATournamentManager::SetInteractionData(const FInteractionData& Value)
{
	InteractionData = Value;
}

