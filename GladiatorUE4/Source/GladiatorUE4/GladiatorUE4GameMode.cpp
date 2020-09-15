// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GladiatorUE4GameMode.h"
#include "GladiatorUE4Character.h"
#include "UObject/ConstructorHelpers.h"

AGladiatorUE4GameMode::AGladiatorUE4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
