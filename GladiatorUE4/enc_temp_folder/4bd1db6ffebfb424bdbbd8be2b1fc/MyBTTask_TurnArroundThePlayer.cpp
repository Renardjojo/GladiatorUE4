// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTTask_TurnArroundThePlayer.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "AIController.h" //AAIController
#include "Kismet/KismetMathLibrary.h" //FindLookAtRotation

/*Debug*/
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage
#include "Containers/UnrealString.h" //FString::Printf

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

	switch (pAIController->MoveToLocation((pSelfPawn->GetActorLocation() - pEnnemyActor->GetActorLocation()).RotateAngleAxis(45, FVector::UpVector) + pEnnemyActor->GetActorLocation()))
	{
		case EPathFollowingRequestResult::AlreadyAtGoal :
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("OK"));
			pSelfPawn->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(pSelfPawn->GetActorLocation(), pEnnemyActor->GetActorLocation()));
			break;
	}

	return EBTNodeResult::Succeeded;
}