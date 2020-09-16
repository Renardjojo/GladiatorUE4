// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GladiatorUE4GameMode.generated.h"

/*Forward declaration*/
class AEnnemy;

UCLASS(minimalapi)
class AGladiatorUE4GameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	uint8 m_numberOfEnnemyAtWolrdInit = 5;

	AActor* EnnemyManager;

public:
	AGladiatorUE4GameMode();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) final;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<AEnnemy> DefaultEnnemyClass;
};



