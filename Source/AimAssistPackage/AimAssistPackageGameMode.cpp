// Copyright Epic Games, Inc. All Rights Reserved.

#include "AimAssistPackageGameMode.h"
#include "AimAssistPackageCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAimAssistPackageGameMode::AAimAssistPackageGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
