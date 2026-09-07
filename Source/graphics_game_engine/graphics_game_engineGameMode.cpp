// Copyright Epic Games, Inc. All Rights Reserved.

#include "graphics_game_engineGameMode.h"
#include "graphics_game_engineCharacter.h"
#include "UObject/ConstructorHelpers.h"

Agraphics_game_engineGameMode::Agraphics_game_engineGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
