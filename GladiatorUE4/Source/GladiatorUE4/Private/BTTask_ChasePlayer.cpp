// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//OwnerComp.GetBlackboardComponent()->GetKeyID();

	return EBTNodeResult::Succeeded;
}