// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GladiatorUE4Character.generated.h"

class UPrimitiveComponent;
struct FHitResult;

UCLASS(config=Game)
class AGladiatorUE4Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	class UMaterialInstanceDynamic* DynMaterial;

protected:

	/** The shield mesh associated with this Character*/
	UPROPERTY(Category = Tools, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* MeshShield;

	/** The shield mesh associated with this Character*/
	UPROPERTY(Category = Tools, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* MeshTool;

	/*life*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	uint8 m_life;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	uint8 m_maxLife;



public:
	AGladiatorUE4Character();

	void BeginPlay() final;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StateMachine)
	bool IsAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StateMachine)
	bool IsBlock;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(BlueprintCallable)
	void StopNotifieDamage();

	UFUNCTION(BlueprintCallable)
	void NotifieDamage();

	/**
	 * @brief CallBack OnComponent overlap of weapon
	 * @param OnComponentBeginOverlap 
	 * @param OverlappedComponent 
	 * @param OtherActor 
	 * @param OtherComp 
	 * @param OtherBodyIndex 
	 * @param SweepResult 
	*/
	UFUNCTION(BlueprintCallable)
	void TryToInflictDammageToEnnemyCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopAttack() noexcept;

	UFUNCTION(BlueprintCallable, Category = "Defense")
	void StopDefense() noexcept;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack() noexcept;
	
	UFUNCTION(BlueprintCallable, Category = "Defense")
	void Block() noexcept;


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
