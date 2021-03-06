// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MoveTo.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "GameFramework/Pawn.h" //APawn
#include "GameFramework/Controller.h" //AController
#include "AIController.h" //AAIController

EBTNodeResult::Type UMyBTTask_MoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const bool rst = isInsidZone(OwnerComp);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(m_flagZoneToStop.SelectedKeyName, rst);

	if (rst)
	{
		return EBTNodeResult::Succeeded;
	}

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void UMyBTTask_MoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//Get self object and cast it to APawn
	APawn* pSelfPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	const float acceptableRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(m_rangeZoneToStop.SelectedKeyName);

	if (!pSelfPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	AAIController* pAIController = Cast<AAIController>(pSelfPawn->GetController());

	if (!pAIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	AActor* pEnnemyActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnnemyTarget"));

	if (!pEnnemyActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	pAIController->MoveToActor(pEnnemyActor, acceptableRadius, m_stopOnOverlap, m_usePathfinding, m_canStrafe);

	const bool rst = isInsidZone(OwnerComp);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(m_flagZoneToStop.SelectedKeyName, rst);
	FinishLatentTask(OwnerComp, rst ? EBTNodeResult::Succeeded : EBTNodeResult::InProgress);
}

bool UMyBTTask_MoveTo::isInsidZone (UBehaviorTreeComponent& OwnerComp)
{
	APawn* pSelfPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	const float acceptableRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(m_rangeZoneToStop.SelectedKeyName);

	if (!pSelfPawn)
	{
		FinishLatentAbort(OwnerComp);
	}

	AActor* pEnnemyActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnnemyTarget"));

	if (!pEnnemyActor)
	{
		FinishLatentAbort(OwnerComp);
	}

	const float zoneSize = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ZoneSize");
	const float squaredDistWithEnnemy = (pEnnemyActor->GetActorLocation() - pSelfPawn->GetActorLocation()).SizeSquared();
	const bool rst = squaredDistWithEnnemy <= (acceptableRadius + zoneSize) * (acceptableRadius + zoneSize);

	return rst;
}