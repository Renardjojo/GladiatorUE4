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

	/*life*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	uint8 m_life;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	uint8 m_maxLife;

public:
	// Sets default values for this character's properties
	AEnnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * @brief Function to inflict dammage to player
	 * @param dammage
	 * @return
	*/
	UFUNCTION(BlueprintCallable, Category = "Life")
	void TakeDammage(uint8 dammage) noexcept;

	/**
	 * @brief Function to heal the player
	 * @param dammage
	 * @return
	*/
	UFUNCTION(BlueprintCallable, Category = "Life")
	void TakeLife(uint8 additionnalLife) noexcept;

	/**
	 * @brief return the life of the player
	 * @return
	*/
	UFUNCTION(BlueprintCallable, Category = "Life")
	FORCEINLINE uint8 GetLife() const noexcept { return m_life; }

};
