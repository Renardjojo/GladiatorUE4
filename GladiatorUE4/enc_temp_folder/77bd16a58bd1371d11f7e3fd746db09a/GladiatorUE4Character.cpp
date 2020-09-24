// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GladiatorUE4Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h" // FTimerManager::SetTimer
#include "GameFramework/CharacterMovementComponent.h" //UCharacterMovementComponent::SetMovementMode
#include "Components/SkeletalMeshComponent.h" //USkeletalMeshComponent
#include "Components/PrimitiveComponent.h" //OnComponentOverlap
#include "Engine/EngineTypes.h" //FHitResult
#include "Ennemy.h" //AEnnemy
#include "Materials/Material.h"//SetMaterial, GetMaterial
#include "Materials/MaterialInstanceDynamic.h" //SetVectorParameterValue, UMaterialInstanceDynamic

/*Debug*/
#include "Engine/GameEngine.h" //AddOnScreenDebugMessage
#include "Containers/UnrealString.h"

//////////////////////////////////////////////////////////////////////////
// AGladiatorUE4Character

AGladiatorUE4Character::AGladiatorUE4Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//create shield
	MeshShield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshShield"));
	MeshShield->SetupAttachment(GetMesh());

	// Create Weapon
	MeshTool = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshTool"));
	MeshTool->SetupAttachment(GetMesh());

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AGladiatorUE4Character::BeginPlay()
{
	Super::BeginPlay();

	FScriptDelegate CollisionBeginOverlapWeapon;
	CollisionBeginOverlapWeapon.BindUFunction(this, TEXT("TryToInflictDammageToEnnemyCallBack"));
	MeshTool->OnComponentBeginOverlap.Add(CollisionBeginOverlapWeapon);

	FScriptDelegate CollisionBeginOverlapShield;
	CollisionBeginOverlapShield.BindUFunction(this, TEXT("BlockAttackCallBack"));
	MeshShield->OnComponentBeginOverlap.Add(CollisionBeginOverlapShield);

	auto Material = GetMesh()->GetMaterial(0);
}

void AGladiatorUE4Character::TryToInflictDammageToEnnemyCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Shield")))
		return;

	AGladiatorUE4Character* pEnnemyActor = Cast<AGladiatorUE4Character>(OtherActor);

	if (pEnnemyActor)
	{
		pEnnemyActor->TakeDammage(1);
	}
}

void AGladiatorUE4Character::BlockAttackCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGladiatorUE4Character* pEnnemyActor = Cast<AGladiatorUE4Character>(OtherActor);

	pEnnemyActor->StopAttack();
	pEnnemyActor->StopInflictDamage();

	AEnnemy* pEnnemy = Cast<AEnnemy>(OtherActor);

	if (pEnnemy)
	{
		pEnnemy->GiveOrderToRetreat();
	}
}

void AGladiatorUE4Character::StopAttack_Implementation()
{
	IsAttack = false;
	Controller->SetIgnoreMoveInput(false);
}

void AGladiatorUE4Character::StopDefense_Implementation()
{
	IsBlock = false;
	Controller->SetIgnoreMoveInput(false);
	MeshShield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGladiatorUE4Character::Attack_Implementation()
{
	IsAttack = true;
	Controller->SetIgnoreMoveInput(true);
}

void AGladiatorUE4Character::Block_Implementation()
{
	IsBlock = true;
	Controller->SetIgnoreMoveInput(true);
	MeshShield->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGladiatorUE4Character::StartInflictDamage() noexcept
{
	MeshTool->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGladiatorUE4Character::StopInflictDamage() noexcept
{
	MeshTool->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AGladiatorUE4Character::CanBlockAttack() noexcept
{
	return IsBlock;
}

void AGladiatorUE4Character::StopNotifieDamage()
{
	GetMesh()->SetVectorParameterValueOnMaterials(FName(TEXT("AdditionalColor")), FVector(FLinearColor::Black));
}

void AGladiatorUE4Character::NotifieDamage()
{
	GetMesh()->SetVectorParameterValueOnMaterials(FName(TEXT("AdditionalColor")), FVector(FLinearColor::Red));
}

void AGladiatorUE4Character::TakeDammage(uint8 dammage) noexcept
{	
	if (m_isImunised)
	{
		return;
	}

	if ((int8)m_life - dammage <= 0)
	{
		/*Kill*/
		Kill();
	}
	else
	{
		ImmuniseForDammage();
		m_life -= dammage;
	}

	FTimerHandle DefaultHandle;
	NotifieDamage();
	GetWorldTimerManager().SetTimer(DefaultHandle, this, &AGladiatorUE4Character::StopNotifieDamage, 1.5f);
}

void AGladiatorUE4Character::Kill() noexcept
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	m_life = 0;
}

void AGladiatorUE4Character::ImmuniseForDammage() noexcept
{
	m_isImunised = true; /*TODO: Can be optimize if disable collision*/

	FTimerHandle DefaultHandle;
	NotifieDamage();
	GetWorldTimerManager().SetTimer(DefaultHandle, this, &AGladiatorUE4Character::StopImunity, 1.5f);
}

void AGladiatorUE4Character::StopImunity() noexcept
{
	m_isImunised = false; /*TODO: Can be optimize if disable collision*/
}

void AGladiatorUE4Character::TakeLife(uint8 additionnalLife) noexcept
{
	if (m_life + additionnalLife > m_maxLife)
	{
		m_life = m_maxLife;
	}
	else
	{
		m_life += additionnalLife;
	}
}