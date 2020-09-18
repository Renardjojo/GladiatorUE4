// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_MoveToContactZone.generated.h"

enum class EZonePlayerState
{
	Outside,
	Inside,
	TooClose
};

/**
 * 
 */
UCLASS()
class GLADIATORUE4_API UMyBTTask_MoveToContactZone : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) final;

	/**
	 * @brief move behaviour
	 * @param OwnerComp 
	 * @return false if error happend
	*/
	bool moveToContactZone(UBehaviorTreeComponent& OwnerComp, APawn* pSelfPawn, AActor* pEnnemyActor, const float acceptableRadius);
	bool runAway(UBehaviorTreeComponent& OwnerComp, APawn* pSelfPawn, AActor* pEnnemyActor, const float acceptableRadius);

	EZonePlayerState checkWhereIsPlayerFromContactZone(UBehaviorTreeComponent& OwnerComp, APawn* pSelfPawn, AActor* pEnnemyActor, const float acceptableRadius);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool    m_stopOnOverlap;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool    m_usePathfinding;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool    m_canStrafe;
};
