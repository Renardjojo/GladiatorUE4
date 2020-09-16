// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_RunAway.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "AIController.h" //AAIController
#include "Kismet/KismetMathLibrary.h" //FindLookAtRotation

EBTNodeResult::Type UMyBTTask_RunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	float acceptableRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("AttackZoneRange");

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

	FVector direction = pSelfPawn->GetActorLocation() - pEnnemyActor->GetActorLocation();
	direction.Normalize();

	pAIController->MoveToLocation((direction * acceptableRadius) + pSelfPawn->GetActorLocation());

	bNotifyTick = true;

	return EBTNodeResult::InProgress;
}


void UMyBTTask_RunAway::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* pSelfPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));

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

	switch (pAIController->GetMoveStatus())
	{
		case EPathFollowingStatus::Idle:
		{
			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(pSelfPawn->GetActorLocation(), pEnnemyActor->GetActorLocation());
			rotation = FRotator::MakeFromEuler(rotation.Euler() * FVector::UpVector);
			pSelfPawn->SetActorRotation(rotation);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsOnFightingZone", false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			break;
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}