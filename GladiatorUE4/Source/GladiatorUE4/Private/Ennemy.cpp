// Fill out your copyright notice in the Description page of Project Settings.


#include "Ennemy.h"
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage
#include "UObject/ScriptDelegates.h"
#include "AIController.h" //AAIController
#include "BrainComponent.h" //BrainComponent
#include "BehaviorTree/BlackboardComponent.h" //BlackboardComponent

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

void AEnnemy::Kill() noexcept
{
	Super::Kill();
	Cast<AAIController>(GetController())->BrainComponent->StopLogic("Death");
}

void AEnnemy::StopAttack_Implementation()
{
	Super::StopAttack_Implementation();
	Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), IsAttack);
}

void AEnnemy::StopDefense_Implementation()
{
	Super::StopDefense_Implementation();
	//Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), IsAttack);
}

void AEnnemy::Attack_Implementation()
{
	Super::Attack_Implementation();
	Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), IsAttack);
}

void AEnnemy::Block_Implementation()
{
	Super::Block_Implementation();
	//Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), IsAttack);
}