// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "MyBTDecorator_CanSeeThePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORUE4_API UMyBTDecorator_CanSeeThePlayer : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
