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
	//Start Validation
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
	//End Validation
	
	//Turn off interaction with table
	DisableInteraction();

	//Start Character setup
	TournamentMainCharacter = OtherCharacter;
	SetupCharacterAtStart();
	//End Character Setup
	
	//Enemy setup (there i init tournament enemy)
	SetupEnemyAtStart();

	TournamentMainCharacter->Enemy = TournamentEnemy;
	TournamentEnemy->MainCharacter = TournamentMainCharacter;
	
	SetCharacterTurn(TournamentMainCharacter);
	
	OnTournamentStart.Broadcast();
	return true;
}

void ATournamentManager::EndTournament()
{
	//Start Validation
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
	//End Validation
	
	//Turn on interaction with table
	EnableInteraction();
	
	SetupCharacterAtEnd();
	

	ResetCharacterTurn();

	OnTournamentEnd.Broadcast();
	ClearCharacterAndEnemy();
}

void ATournamentManager::SetupCharacterAtStart()
{
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
			LookActionEventBinding = &EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, TournamentMainCharacter, &AShotTillDeathCharacter::Look);
			InteractActionEventBinding = &EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, TournamentMainCharacter, &AShotTillDeathCharacter::Interact);
			ExitActionEventBinding = &EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Triggered, this, &ATournamentManager::EndTournament);
		}
	}
	
	TournamentMainCharacter->SetActorLocation(MainCharacterPosition);
	TournamentMainCharacter->SetActorRotation(MainCharacterRotation);
}

void ATournamentManager::SetupEnemyAtStart()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	TournamentEnemy = GetWorld()->SpawnActor<AEnemy>(TypeOfEnemy, SpawnInfo);
	TournamentEnemy->SetActorLocation(EnemyPosition);
	TournamentEnemy->SetActorRotation(EnemyRotation);
}

void ATournamentManager::SetupCharacterAtEnd()
{
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
}

void ATournamentManager::SetupEnemyAtEnd()
{
	
}

void ATournamentManager::ClearCharacterAndEnemy()
{
	TournamentMainCharacter->Enemy = nullptr;
	TournamentEnemy->MainCharacter = nullptr;
	TournamentMainCharacter = nullptr;
	GetWorld()->DestroyActor(TournamentEnemy);
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
	if(!IsValid(TurnCharacter))
	{
		return;
	}
	
	CurrentCharacter = TurnCharacter;
	if(TurnCharacter == TournamentMainCharacter)
	{
		TournamentMainCharacter->ChangeCharacterState(InTournamentCharacterTurn);
		TournamentEnemy->ChangeCharacterState(InTournamentCharacterTurn);
		TournamentMainCharacter->SetCanTakeItem(true);
		TournamentEnemy->SetCanTakeItem(false);
		OnTournamentCharacterTurn.Broadcast();
	}
	else
	{
		TournamentMainCharacter->ChangeCharacterState(InTournamentEnemyTurn);
		TournamentEnemy->ChangeCharacterState(InTournamentEnemyTurn);
		TournamentMainCharacter->SetCanTakeItem(false);
		TournamentEnemy->SetCanTakeItem(true);
		OnTournamentEnemyTurn.Broadcast();
	}
}

void ATournamentManager::ResetCharacterTurn()
{
	CurrentCharacter = nullptr;
	TournamentMainCharacter->ChangeCharacterState(Default);
	TournamentEnemy->ChangeCharacterState(Default);
	TournamentMainCharacter->SetCanTakeItem(true);
	TournamentEnemy->SetCanTakeItem(true);
}

void ATournamentManager::SwapCharacterTurn()
{
	if(CurrentCharacter == TournamentMainCharacter)
	{
		CurrentCharacter = TournamentEnemy;
		TournamentMainCharacter->ChangeCharacterState(InTournamentEnemyTurn);
		TournamentEnemy->ChangeCharacterState(InTournamentEnemyTurn);
		TournamentMainCharacter->SetCanTakeItem(false);
		TournamentEnemy->SetCanTakeItem(true);
		OnTournamentEnemyTurn.Broadcast();
	}
	else
	{
		CurrentCharacter = TournamentMainCharacter;
		TournamentMainCharacter->ChangeCharacterState(InTournamentCharacterTurn);
		TournamentEnemy->ChangeCharacterState(InTournamentCharacterTurn);
		TournamentMainCharacter->SetCanTakeItem(true);
		TournamentEnemy->SetCanTakeItem(false);
		OnTournamentCharacterTurn.Broadcast();
	}
}



