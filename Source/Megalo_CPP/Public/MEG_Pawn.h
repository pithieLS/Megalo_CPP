// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MEG_Pawn.generated.h"

class USceneComponent;
class UCameraComponent;
class UFloatingPawnMovement;
class AMEG_GM;

UCLASS()
class MEGALO_CPP_API AMEG_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMEG_Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float MovementDelta);
	void MoveRight(float MovementDelta);
	void MoveUp(float MovementDelta);
	void OnRotatePressed();

	AMEG_GM* GameMode;

	UPROPERTY(EditAnywhere, Category = "ZoomCamera")
	float MaxHeight = 1500;

	UPROPERTY(EditAnywhere, Category = "ZoomCamera")
	float MinHeight = 150;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UFloatingPawnMovement* MovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
