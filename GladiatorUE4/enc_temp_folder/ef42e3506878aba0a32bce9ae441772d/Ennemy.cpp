// Fill out your copyright notice in the Description page of Project Settings.


#include "Ennemy.h"
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage
#include "UObject/ScriptDelegates.h"
#include "AIController.h" //AAIController
#include "BrainComponent.h" //BrainComponent
#include "BehaviorTree/BlackboardComponent.h" //BlackboardComponent
#include "GameFramework/CharacterMovementComponent.h" //UCharacterMovementComponent

// Sets default values
AEnnemy::AEnnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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
	//GetCharacterMovement()->Ignor
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

void AEnnemy::GiveOrderToCharge() noexcept
{
	IsCharge = true;
	Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool(TEXT("IsCharge"), IsCharge);
}

void AEnnemy::GiveOrderToRetreat() noexcept
{
	IsCharge = false;
	Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool(TEXT("IsCharge"), IsCharge);
}