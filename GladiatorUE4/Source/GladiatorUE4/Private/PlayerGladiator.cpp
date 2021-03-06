// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGladiator.h"

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
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

APlayerGladiator::APlayerGladiator()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerGladiator::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerGladiator::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerGladiator::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerGladiator::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerGladiator::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlayerGladiator::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APlayerGladiator::TouchStopped);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerGladiator::Attack);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &APlayerGladiator::Block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &APlayerGladiator::StopDefense);
}

void APlayerGladiator::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void APlayerGladiator::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void APlayerGladiator::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerGladiator::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerGladiator::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerGladiator::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerGladiator::Kill() noexcept
{
	Super::Kill();
}

void APlayerGladiator::StopAttack_Implementation()
{
	Super::StopAttack_Implementation();
}

void APlayerGladiator::StopDefense_Implementation()
{
	Super::StopDefense_Implementation();
}

void APlayerGladiator::Attack_Implementation()
{
	Super::Attack_Implementation();
}

void APlayerGladiator::Block_Implementation()
{
	Super::Block_Implementation();
}

// Called every frame
void APlayerGladiator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}