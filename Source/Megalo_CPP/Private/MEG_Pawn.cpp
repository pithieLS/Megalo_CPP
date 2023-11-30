// Fill out your copyright notice in the Description page of Project Settings.


#include "MEG_Pawn.h"

#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MEG_GM.h"

// Sets default values
AMEG_Pawn::AMEG_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	if (!ensure(SpringArmComponent != nullptr))
		return;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	if (!ensure(CameraComponent != nullptr))
		return;
	CameraComponent->SetupAttachment(SpringArmComponent);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	if (!ensure(MovementComponent != nullptr))
		return;
}

// Called when the game starts or when spawned
void AMEG_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
}

void AMEG_Pawn::MoveForward(float MovementDelta)
{
	FVector ActorLoc = GetActorLocation();

	if (ActorLoc.Y <= GameMode->BoardLimits.MaxU && MovementDelta > 0)
	{
		SetActorLocation(FVector(ActorLoc.X, GameMode->BoardLimits.MaxU, ActorLoc.Z));
		return;
	}
	else if (ActorLoc.Y >= GameMode->BoardLimits.MaxD && MovementDelta < 0)
	{
		SetActorLocation(FVector(ActorLoc.X, GameMode->BoardLimits.MaxD, ActorLoc.Z));
		return;
	}

	AddMovementInput(GetActorForwardVector(), MovementDelta);
}

void AMEG_Pawn::MoveRight(float MovementDelta)
{
	FVector ActorLoc = GetActorLocation();

	if (ActorLoc.X >= GameMode->BoardLimits.MaxR && MovementDelta > 0)
	{
		SetActorLocation(FVector(GameMode->BoardLimits.MaxR, ActorLoc.Y, ActorLoc.Z));
		return;
	}
	else if (ActorLoc.X <= GameMode->BoardLimits.MaxL && MovementDelta < 0)
	{
		SetActorLocation(FVector(GameMode->BoardLimits.MaxL, ActorLoc.Y, ActorLoc.Z));
		return;
	}

	AddMovementInput(GetActorRightVector(), MovementDelta);
}

void AMEG_Pawn::MoveUp(float MovementDelta)
{
	// ScrollSpeed is proportional to the height of the camera
	const float ScrollSpeed = SpringArmComponent->TargetArmLength * 0.1f;

	float NewLenght = SpringArmComponent->TargetArmLength + ScrollSpeed * MovementDelta;

	NewLenght = FMath::Clamp(NewLenght, MinLenght, MaxLenght);
	const float NewRotationY = FMath::GetMappedRangeValueClamped(FVector2D(MinLenght, MaxLenght), FVector2D(MinRotation, MaxRotation), NewLenght);

	SpringArmComponent->TargetArmLength = NewLenght;
	SpringArmComponent->SetRelativeRotation(FRotator(NewRotationY, 0, 0));

}

void AMEG_Pawn::OnRotatePressed()
{
	GameMode->OnRotatePressedDelegate.ExecuteIfBound();
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
	PlayerInputComponent->BindAxis("MoveUp", this, &AMEG_Pawn::MoveUp);
	PlayerInputComponent->BindAction("RotateCard", IE_Pressed, this, &AMEG_Pawn::OnRotatePressed);
}

