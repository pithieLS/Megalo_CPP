// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MEG_CardPlacer.generated.h"

UCLASS()
class MEGALO_CPP_API AMEG_CardPlacer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMEG_CardPlacer();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnMeshClicked(UPrimitiveComponent* Component, FKey ButtonPressed);

	FVector2D Coords;
protected :
	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComponent;
};