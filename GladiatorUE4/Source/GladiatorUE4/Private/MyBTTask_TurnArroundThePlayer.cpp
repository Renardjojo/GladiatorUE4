// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTTask_TurnArroundThePlayer.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "AIController.h" //AAIController
#include "Kismet/KismetMathLibrary.h" //FindLookAtRotation

EBTNodeResult::Type UMyBTTask_TurnArroundThePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	pAIController->MoveToLocation((pSelfPawn->GetActorLocation() - pEnnemyActor->GetActorLocation()).RotateAngleAxis(75, FVector::UpVector) + pEnnemyActor->GetActorLocation());

	bNotifyTick = true;

	return EBTNodeResult::InProgress;
}


void UMyBTTask_TurnArroundThePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		case EPathFollowingStatus::Paused:
		case EPathFollowingStatus::Waiting:
			pAIController->MoveToLocation((pSelfPawn->GetActorLocation() - pEnnemyActor->GetActorLocation()).RotateAngleAxis(-75, FVector::UpVector) + pEnnemyActor->GetActorLocation());
			break;

		case EPathFollowingStatus::Idle:
		{
			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(pSelfPawn->GetActorLocation(), pEnnemyActor->GetActorLocation());
			rotation = FRotator::MakeFromEuler(rotation.Euler() * FVector::UpVector);
			pSelfPawn->SetActorRotation(rotation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			break;
		}

	}

	if (pAIController->HasPartialPath())
	{
		pAIController->MoveToLocation((pSelfPawn->GetActorLocation() - pEnnemyActor->GetActorLocation()).RotateAngleAxis(-75, FVector::UpVector) + pEnnemyActor->GetActorLocation());
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}