// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#define SHOW_PRIVATE UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));

#include "MyBTTask_MoveTo.generated.h"


/**
 * 
 */
UCLASS()
class GLADIATORUE4_API UMyBTTask_MoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	SHOW_PRIVATE
	float	m_acceptableRadius;

	SHOW_PRIVATE
	bool    m_stopOnOverlap;

	SHOW_PRIVATE
	bool    m_usePathfinding;

	SHOW_PRIVATE
	bool    m_canStrafe;
};
