// Fill out your copyright notice in the Description page of Project Settings.


#include "Tournament/TournamentManager.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionComponent.h"
#include "Enemy/Enemy.h"
#include "ShotTillDeath/ShotTillDeathCharacter.h"


// Sets default values
ATournamentManager::ATournamentManager()
{
	PrimaryActorTick.bCanEverTick = true;

	TournamentRootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(ToRawPtr(TournamentRootComponent));

	MainCharacterTransformComponent = CreateDefaultSubobject<USceneComponent>("MainCharacterTransformComponent");
	EnemyTransformComponent = CreateDefaultSubobject<USceneComponent>("EnemyTransformComponent");

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
	SetCharacterTurn(OtherCharacter);

	TournamentMainCharacter->ClearPlayerInputs();

	DisableInteraction();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(TournamentMainCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(TournamentMappingContext, 2);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// BindAction
			LookActionEventBinding = &EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, TournamentMainCharacter, &AShotTillDeathCharacter::Look);
			InteractActionEventBinding = &EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, TournamentMainCharacter, &AShotTillDeathCharacter::Interact);
			ExitActionEventBinding = &EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Triggered, this, &ATournamentManager::EndTournament);
		}
	}
	
	TournamentMainCharacter->SetActorLocation(MainCharacterPosition);
	TournamentMainCharacter->SetActorRotation(MainCharacterRotation);

	//TODO Create Enemy
	SetupEnemy();
	
	OnTournamentStart.Broadcast();
	
	return true;
}

void ATournamentManager::EndTournament()
{
	if(!GetWorld())
	{
		return;
	}
	
	if(!IsValid(TournamentMainCharacter))
	{
		UE_LOG(LogTemp, Display, TEXT("Not valid TournamentMainCharacter"));
		return;
	}
	if(!IsValid(TournamentEnemy))
	{
		UE_LOG(LogTemp, Display, TEXT("Not valid TournamentEnemy"));
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
			EnhancedInputComponent->RemoveBinding(*LookActionEventBinding);
			EnhancedInputComponent->RemoveBinding(*InteractActionEventBinding);
			EnhancedInputComponent->RemoveBinding(*ExitActionEventBinding);
		}
	}

	TournamentMainCharacter->SetupPlayerInputs();
	
	OnTournamentEnd.Broadcast();

	ResetCharacterTurn();
	TournamentMainCharacter = nullptr;

	GetWorld()->DestroyActor(TournamentEnemy);
}

void ATournamentManager::SetupEnemy()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	TournamentEnemy = GetWorld()->SpawnActor<AEnemy>(TypeOfEnemy, SpawnInfo);
	TournamentEnemy->SetActorLocation(EnemyPosition);
	TournamentEnemy->SetActorRotation(EnemyRotation);
}


void ATournamentManager::SetCurrentPositionForMainCharacter()
{
	if(MainCharacterTransformComponent)
	{
		MainCharacterPosition = MainCharacterTransformComponent->GetComponentLocation();
		MainCharacterRotation = MainCharacterTransformComponent->GetComponentRotation();
	}
}

void ATournamentManager::SetCurrentPositionForEnemy()
{
	if(EnemyTransformComponent)
	{
		EnemyPosition = EnemyTransformComponent->GetComponentLocation();
		EnemyRotation = EnemyTransformComponent->GetComponentRotation();
	}
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

void ATournamentManager::SetCharacterTurn(AShotTillDeathBaseCharacter* TurnCharacter)
{
	CurrentCharacter = TurnCharacter;
}

void ATournamentManager::ResetCharacterTurn()
{
	CurrentCharacter = nullptr;
}

void ATournamentManager::SwapCharacterTurn()
{
	if(CurrentCharacter == TournamentMainCharacter)
	{
		CurrentCharacter = TournamentEnemy;
	}
	else
	{
		CurrentCharacter = TournamentMainCharacter;
	}
}



