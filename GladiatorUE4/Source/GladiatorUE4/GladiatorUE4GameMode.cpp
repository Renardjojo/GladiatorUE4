// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GladiatorUE4GameMode.h"
#include "GladiatorUE4Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Ennemy.h"
#include "GameFramework/Character.h"

/*Debug*/
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage

AGladiatorUE4GameMode::AGladiatorUE4GameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/

	static ConstructorHelpers::FClassFinder<AEnnemy> EnnemyBPClass(TEXT("/Game/Ennemy/Ennemy"));
	if (EnnemyBPClass.Class != NULL)
	{
		DefaultEnnemyClass = EnnemyBPClass.Class;
	}

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AGladiatorUE4GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, false);

	for (size_t i = 0; i < m_numberOfEnnemyAtWolrdInit; i++)
	{
		Location = FVector{ FMath::RandPointInCircle(1000.f), 200.0f };
		Rotation = FRotator{ 0.0f, 0.0f, 0.0f };
		
		FAttachmentTransformRules aRules(EAttachmentRule::KeepRelative, true);

		AEnnemy* pEnnemy = GetWorld()->SpawnActor<AEnnemy>(DefaultEnnemyClass.Get(), Location, Rotation, SpawnInfo);
		EnnemyManager.Add(pEnnemy);	
	}
}

void AGladiatorUE4GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!waitingForNextAttack && CurrentAttackingEnnemy != nullptr && !CurrentAttackingEnnemy->IsCharging())
	{
		waitingForNextAttack = true;
	}

	if (waitingForNextAttack)
	{
		delayBeforNextAttackCount += DeltaSeconds;

		if (delayBeforNextAttackCount >= delayBeforNextAttack)
		{
			waitingForNextAttack = false;
			delayBeforNextAttackCount -= delayBeforNextAttack;
		}
		else
		{
			return;
		}
	}

	if (CurrentAttackingEnnemy != nullptr && (CurrentAttackingEnnemy->IsPendingKill() || CurrentAttackingEnnemy->GetLife() == 0))
	{
		CurrentAttackingEnnemy = nullptr;
	}

	if (CurrentAttackingEnnemy == nullptr || !CurrentAttackingEnnemy->IsCharging())
	{
		CurrentAttackingEnnemy = nullptr;

		for (size_t i = 0; i < EnnemyManager.Num(); i++)
		{
			if (EnnemyManager[i]->IsPendingKill() || EnnemyManager[i]->GetLife() == 0)
			{
				EnnemyManager.RemoveAtSwap(i);

				if (i != 0)
					i--;
			}
		}

		for (AEnnemy* pEnnemie : EnnemyManager)
		{
			if (pEnnemie->CanAttack)
			{
				CurrentAttackingEnnemy = pEnnemie;
				pEnnemie->GiveOrderToCharge();

				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Ok %d"), CurrentAttackingEnnemy->IsAttack));
				break;
			}
		}
	}
}