// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Jam2018GameMode.h"
#include "Jam2018HUD.h"
#include "Jam2018Character.h"
#include "UObject/ConstructorHelpers.h"

AJam2018GameMode::AJam2018GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AJam2018HUD::StaticClass();
}
