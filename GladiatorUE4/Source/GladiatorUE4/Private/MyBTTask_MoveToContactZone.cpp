// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MoveToContactZone.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "GameFramework/Pawn.h" //APawn
#include "GameFramework/Controller.h" //AController
#include "AIController.h" //AAIController
#include "Kismet/KismetMathLibrary.h" //FindLookAtRotation

EBTNodeResult::Type UMyBTTask_MoveToContactZone::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* pSelfPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	const float acceptableRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("AttackZoneRange");

	if (!pSelfPawn)
	{
		EBTNodeResult::Aborted;
	}

	AActor* pEnnemyActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnnemyTarget"));

	if (!pEnnemyActor)
	{
		EBTNodeResult::Aborted;
	}

	const EZonePlayerState playerLocationFromContactZone = checkWhereIsPlayerFromContactZone(OwnerComp, pSelfPawn, pEnnemyActor, acceptableRadius);

	switch (playerLocationFromContactZone)
	{
	case EZonePlayerState::TooClose:
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsOnContactZone", false);
		pSelfPawn->bUseControllerRotationYaw = false;
		return runAway(OwnerComp, pSelfPawn, pEnnemyActor, acceptableRadius) ? EBTNodeResult::Failed : EBTNodeResult::Aborted;

	case EZonePlayerState::Inside:
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsOnContactZone", true);
		pSelfPawn->bUseControllerRotationYaw = false;
		return EBTNodeResult::Succeeded;

	case EZonePlayerState::Outside:
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsOnContactZone", false);
		pSelfPawn->bUseControllerRotationYaw = false;
		return moveToContactZone(OwnerComp, pSelfPawn, pEnnemyActor, acceptableRadius) ? EBTNodeResult::Failed : EBTNodeResult::Aborted;

	default:
		return EBTNodeResult::Failed;
	}
}

bool UMyBTTask_MoveToContactZone::moveToContactZone(UBehaviorTreeComponent& OwnerComp, APawn* pSelfPawn, AActor* pEnnemyActor, const float acceptableRadius)
{
	AAIController* pAIController = Cast<AAIController>(pSelfPawn->GetController());
	const float zoneSize = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ZoneSize");

	if (!pAIController)
	{
		return false;
	}

	pAIController->MoveToActor(pEnnemyActor, acceptableRadius - zoneSize, m_stopOnOverlap, m_usePathfinding, m_canStrafe);
	rotateToLookPlayer(OwnerComp, pSelfPawn, pEnnemyActor);

	return true;
}

bool UMyBTTask_MoveToContactZone::runAway(UBehaviorTreeComponent& OwnerComp, APawn* pSelfPawn, AActor* pEnnemyActor, const float acceptableRadius)
{
	AAIController* pAIController = Cast<AAIController>(pSelfPawn->GetController());
	const float zoneSize = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ZoneSize");

	if (!pAIController)
	{
		return false;
	}

	FVector direction = pSelfPawn->GetActorLocation() - pEnnemyActor->GetActorLocation();
	direction.Normalize();

	pAIController->MoveToLocation((direction * (acceptableRadius - zoneSize)) + pSelfPawn->GetActorLocation());

	rotateToLookPlayer(OwnerComp, pSelfPawn, pEnnemyActor);

	return true;
}

EZonePlayerState UMyBTTask_MoveToContactZone::checkWhereIsPlayerFromContactZone(UBehaviorTreeComponent& OwnerComp, APawn* pSelfPawn, AActor* pEnnemyActor, const float acceptableRadius)
{
	const float zoneSize = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ZoneSize");
	const float squaredDistWithEnnemy = (pEnnemyActor->GetActorLocation() - pSelfPawn->GetActorLocation()).SizeSquared();

	if (squaredDistWithEnnemy <= (acceptableRadius - zoneSize) * (acceptableRadius - zoneSize))
	{
		return EZonePlayerState::TooClose;
	}
	else if (squaredDistWithEnnemy >= (acceptableRadius + zoneSize) * (acceptableRadius + zoneSize))
	{
		return EZonePlayerState::Outside;
	}
	else
	{
		return EZonePlayerState::Inside;
	}
}

void UMyBTTask_MoveToContactZone::rotateToLookPlayer(UBehaviorTreeComponent& OwnerComp, APawn* pSelfPawn, AActor* pEnnemyActor)
{
	//Alway face to the player
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(pSelfPawn->GetActorLocation(), pEnnemyActor->GetActorLocation());
	rotation = FRotator::MakeFromEuler(rotation.Euler() * FVector::UpVector);
	pSelfPawn->SetActorRotation(rotation);
}