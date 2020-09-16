// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTDecorator_CanSeeThePlayer.h"
#include "BehaviorTree/BlackboardComponent.h" //UBlackboardComponent
#include "Components/PrimitiveComponent.h"

/*Debug*/
//#include "DrawDebugHelpers.h" //DrawDebugLine
//#include "Engine/GameEngine.h" //AddOnScreenDebugMessage
//#include "Containers/UnrealString.h" //FString::Printf

bool UMyBTDecorator_CanSeeThePlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* pSelfActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));

	FHitResult OutHit;
	FVector Start = pSelfActor->GetActorLocation();

	FVector ForwardVector = pSelfActor->GetActorForwardVector();
	FVector End = ((ForwardVector * 500.f) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(pSelfActor);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1, 0, 1);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Pawn, CollisionParams, FCollisionResponseParams::DefaultResponseParam))
	{
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %d"), OutHit.GetComponent()->ComponentHasTag("Player")));

		return OutHit.GetComponent()->ComponentHasTag("Player");
	}

	return false;
}