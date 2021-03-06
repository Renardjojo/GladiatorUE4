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

	/**
	 * @brief If set, gladiator will charge and then attack
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StateMachine)
	bool IsCharge = false;

protected:

	virtual void Kill() noexcept override;

	virtual void StopAttack_Implementation()  override;

	virtual void StopDefense_Implementation()  override;

	virtual void Attack_Implementation() override;

	virtual void Block_Implementation()  override;

public:	

	UFUNCTION(BlueprintCallable, Category = "State")
	void GiveOrderToCharge() noexcept;

	UFUNCTION(BlueprintCallable, Category = "State")
	void GiveOrderToRetreat() noexcept;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsCharging() const noexcept { return IsCharge; }
};
