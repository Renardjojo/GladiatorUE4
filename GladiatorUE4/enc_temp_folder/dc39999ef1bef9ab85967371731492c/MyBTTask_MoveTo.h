// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_MoveTo.generated.h"

UCLASS()
class GLADIATORUE4_API UMyBTTask_MoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) final;

	void TickTask (UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) final;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector m_rangeZoneToStop;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector m_flagZoneToStop;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool    m_stopOnOverlap;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool    m_usePathfinding;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool    m_canStrafe;
};
