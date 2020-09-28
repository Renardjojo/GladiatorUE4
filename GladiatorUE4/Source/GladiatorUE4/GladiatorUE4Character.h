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

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool m_isImunised = false;

public:
	AGladiatorUE4Character();

	void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StateMachine)
	bool IsAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StateMachine)
	bool IsBlock;

protected:

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

	UFUNCTION(BlueprintCallable)
	void BlockAttackCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	
	virtual void Kill() noexcept;

	UFUNCTION(BlueprintCallable, Category = "State")
	void DoNotTakeDammage() noexcept;

	UFUNCTION(BlueprintCallable, Category = "State")
	void ImmuniseForDammage() noexcept;

	UFUNCTION(BlueprintCallable, Category = "State")
	void StopImunity() noexcept;

	UFUNCTION(BlueprintCallable, Category = "State")
	void NotifieDamage();

	UFUNCTION(BlueprintCallable, Category = "State")
	void StopNotifieDamage();

	UFUNCTION(BlueprintCallable, Category = "State")
	void StartInflictDamage() noexcept;

	UFUNCTION(BlueprintCallable, Category = "State")
	void StopInflictDamage() noexcept;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool CanBlockAttack() noexcept;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopAttack();
	virtual void StopAttack_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopDefense();
	virtual void StopDefense_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack();
	virtual void Attack_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Block();
	virtual void Block_Implementation();

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

	UFUNCTION(BlueprintCallable, Category = "Life")
	FORCEINLINE float GetLifeRatio() const noexcept { return m_life / (float)m_maxLife; }

};

