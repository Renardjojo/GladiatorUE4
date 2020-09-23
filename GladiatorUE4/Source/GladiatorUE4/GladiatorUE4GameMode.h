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
	uint8 m_numberOfEnnemyAtWolrdInit = 7;

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnnemy*> EnnemyManager;

private :
	AEnnemy* CurrentAttackingEnnemy = nullptr;

	float delayBeforNextAttackCount = 0.f;
	bool waitingForNextAttack = true;

	UPROPERTY(EditAnywhere)
	float delayBeforNextAttack = 3.f;
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<AEnnemy> DefaultEnnemyClass;

	AGladiatorUE4GameMode();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) final;

	void Tick(float DeltaSeconds) final;

};



