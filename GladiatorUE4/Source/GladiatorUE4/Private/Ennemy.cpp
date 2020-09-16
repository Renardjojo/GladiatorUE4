// Fill out your copyright notice in the Description page of Project Settings.


#include "Ennemy.h"
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage
#include "UObject/ScriptDelegates.h"

// Sets default values
AEnnemy::AEnnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	m_AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionTest"));

	FScriptDelegate deleg;
	//deleg.BindUFunction(AEnnemy::SeeAndTryToChassThePlayerCallBack, name);
	deleg.BindUFunction(this, "SeeAndTryToChassThePlayerCallBack");

	m_AIPerception->OnTargetPerceptionUpdated.Add(deleg);
}

void AEnnemy::SeeAndTryToChassThePlayerCallBack(AActor* Actor, FAIStimulus Stimulus)
{}

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

