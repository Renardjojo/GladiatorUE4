// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTDecorator_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UMyBTDecorator_BlackboardBase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsBool("Run");
}