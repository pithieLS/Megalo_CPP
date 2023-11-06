// Fill out your copyright notice in the Description page of Project Settings.


#include "MEG_Pawn.h"

#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
AMEG_Pawn::AMEG_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	if (!ensure(CameraComponent != nullptr))
		return;
	CameraComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	if (!ensure(MovementComponent != nullptr))
		return;
}

// Called when the game starts or when spawned
void AMEG_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMEG_Pawn::MoveForward(float MovementDelta)
{
	AddMovementInput(GetActorForwardVector(), MovementDelta);
}

void AMEG_Pawn::MoveRight(float MovementDelta)
{
	AddMovementInput(GetActorRightVector(), MovementDelta);

}

void AMEG_Pawn::LookUp(float X_Axis)
{
	//AddControllerYawInput()
}

// Called every frame
void AMEG_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMEG_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMEG_Pawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMEG_Pawn::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMEG_Pawn::LookUp);
}

