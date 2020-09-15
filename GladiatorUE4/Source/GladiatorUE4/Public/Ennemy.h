// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h" //AiPerception


#include "Ennemy.generated.h"

/*Forward declaration*/


UCLASS()
class GLADIATORUE4_API AEnnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AIPerception, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* m_AIPerception;
	

public:
	// Sets default values for this character's properties
	AEnnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Callback called by OnTargetPerceptionUpdated of AIPerception
	 * @param Actor 
	 * @param Stimulus 
	*/
	UFUNCTION()
	void SeeAndTryToChassThePlayerCallBack(AActor* Actor, FAIStimulus Stimulus);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
