// Fill out your copyright notice in the Description page of Project Settings.


#include "Ennemy.h"
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage
#include "UObject/ScriptDelegates.h"

// Sets default values
AEnnemy::AEnnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnnemy::TakeDammage(uint8 dammage) noexcept
{
	if ((int8)m_life - dammage < 0)
	{
		m_life = 0;
	}
	else
	{
		m_life -= dammage;
	}
}

void AEnnemy::TakeLife(uint8 additionnalLife) noexcept
{
	if (m_life + additionnalLife > m_maxLife)
	{
		m_life = m_maxLife;
	}
	else
	{
		m_life += additionnalLife;
	}
}