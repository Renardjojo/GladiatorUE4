// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GladiatorUE4GameMode.h"
#include "GladiatorUE4Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Ennemy.h"
#include "GameFramework\Character.h"


/*Debug*/
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage

AGladiatorUE4GameMode::AGladiatorUE4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> EnnemyBPClass(TEXT("/Game/Ennemy/Ennemy"));
	if (EnnemyBPClass.Class != NULL)
	{
		DefaultEnnemyClass = EnnemyBPClass.Class;
	}
}

void AGladiatorUE4GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	EnnemyManager = GetWorld()->SpawnActor<AActor>(Location, Rotation, SpawnInfo);
	EnnemyManager->SetActorLabel("EnnemyContenor");

	for (size_t i = 0; i < m_numberOfEnnemyAtWolrdInit; i++)
	{
		Location = FVector{ FMath::RandPointInCircle(1000.f), 200.0f };
		Rotation = FRotator{ 0.0f, 0.0f, 0.0f };
		
		FAttachmentTransformRules aRules(EAttachmentRule::KeepRelative, true);

		AEnnemy* pEnnemy = GetWorld()->SpawnActor<AEnnemy>(DefaultEnnemyClass.Get(), Location, Rotation, SpawnInfo);
		
		if (pEnnemy)
		{
			pEnnemy->AttachToActor(EnnemyManager, aRules);
		}		
	}
}
