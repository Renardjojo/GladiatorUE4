// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MoveTo.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "GameFramework/Pawn.h" //APawn
#include "GameFramework/Controller.h" //AController
#include "AIController.h" //AAIController


EBTNodeResult::Type UMyBTTask_MoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get self object and cast it to APawn
	APawn* pSelfPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));

	if (!pSelfPawn)
	{
		return EBTNodeResult::Failed;
	}

	AAIController* pAIController = Cast<AAIController>(pSelfPawn->GetController());

	if (!pAIController)
	{
		return EBTNodeResult::Failed;
	}

	AActor* pEnnemyActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnnemyTarget"));

	if (!pEnnemyActor)
	{
		return EBTNodeResult::Failed;
	}

	pAIController->MoveToActor(pEnnemyActor, m_acceptableRadius, m_stopOnOverlap, m_usePathfinding, m_canStrafe);

	return EBTNodeResult::Succeeded;
}