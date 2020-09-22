// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorUE4/GladiatorUE4Character.h"

#include "Ennemy.generated.h"

/*Forward declaration*/


UCLASS()
class GLADIATORUE4_API AEnnemy : public AGladiatorUE4Character
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StateMachine)
	bool CanAttack = false;

protected:

	virtual void Kill() noexcept override;

	virtual void StopAttack_Implementation()  override;

	virtual void StopDefense_Implementation()  override;

	virtual void Attack_Implementation() override;

	virtual void Block_Implementation()  override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
