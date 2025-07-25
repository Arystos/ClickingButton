// Copyright Epic Games, Inc. All Rights Reserved.

#include "ClickingButtonGameGameMode.h"
#include "ClickingButtonGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AClickingButtonGameGameMode::AClickingButtonGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
