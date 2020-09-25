// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MoveToContactZone.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "GameFramework/Pawn.h" //APawn
#include "GameFramework/Controller.h" //AController
#include "AIController.h" //AAIController
#include "Kismet/KismetMathLibrary.h" //FindLookAtRotation
#include "Ennemy.h" //AEnnemy

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
		Cast<AEnnemy>(pSelfPawn)->CanAttack = true;

		pSelfPawn->bUseControllerRotationYaw = true;
		Cast<AAIController>(pSelfPawn->GetController())->SetFocus(pEnnemyActor);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TooClose"));
		return runAway(OwnerComp, pSelfPawn, pEnnemyActor, acceptableRadius) ? EBTNodeResult::Failed : EBTNodeResult::Aborted;

	case EZonePlayerState::Inside:
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsOnContactZone", true);
		Cast<AEnnemy>(pSelfPawn)->CanAttack = true;

		pSelfPawn->bUseControllerRotationYaw = true;
		Cast<AAIController>(pSelfPawn->GetController())->SetFocus(pEnnemyActor);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inside"));
		return EBTNodeResult::Succeeded;

	case EZonePlayerState::Outside:
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsOnContactZone", false);
		Cast<AEnnemy>(pSelfPawn)->CanAttack = false;

		pSelfPawn->bUseControllerRotationYaw = false;
		Cast<AAIController>(pSelfPawn->GetController())->ClearFocus(EAIFocusPriority::Default);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Outside"));
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

	pAIController->MoveToActor(pEnnemyActor, acceptableRadius - zoneSize, false, false, false);

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

	//EPathFollowingRequestResult::Type MoveRequestResult;

	//do
	//{
		/*MoveRequestResult = */pAIController->MoveToLocation((direction * zoneSize) + pSelfPawn->GetActorLocation());
		//direction.RotateAngleAxis(10, FVector::UpVector);

	//} while (MoveRequestResult == EPathFollowingRequestResult::Failed);


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
	rotation.Pitch = 0;
	rotation.Roll = 0;
	pSelfPawn->SetActorRotation(rotation);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *pSelfPawn->GetActorRotation().ToString()));
}