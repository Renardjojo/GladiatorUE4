// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Attack.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "GameFramework/Pawn.h" //APawn
#include "GameFramework/Controller.h" //AController
#include "AIController.h" //AAIController
#include "GladiatorUE4/GladiatorUE4Character.h"

/*Debug*/
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage

EBTNodeResult::Type UMyBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* pSelfPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	const float acceptableRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("FigthZoneRange");

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

	pAIController->MoveToActor(pEnnemyActor, acceptableRadius);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsOnFightingZone", true);

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void UMyBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Get self object and cast it to APawn
	APawn* pSelfPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	const float acceptableRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("FigthZoneRange");

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

	if (pAIController->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		Cast<AGladiatorUE4Character>(pSelfPawn)->Attack();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}
