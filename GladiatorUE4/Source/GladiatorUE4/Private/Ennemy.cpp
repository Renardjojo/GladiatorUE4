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