// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShotTillDeathGameMode.h"
#include "ShotTillDeathCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShotTillDeathGameMode::AShotTillDeathGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
