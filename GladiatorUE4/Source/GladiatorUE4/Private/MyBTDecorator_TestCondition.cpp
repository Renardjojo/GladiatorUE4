// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTDecorator_TestCondition.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage

bool UMyBTDecorator_TestCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(m_condition.SelectedKeyName);
}