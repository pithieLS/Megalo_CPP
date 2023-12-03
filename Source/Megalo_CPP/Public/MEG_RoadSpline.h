// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MEG_RoadSpline.generated.h"

class USplineComponent;
class AMEG_GridCell;
class AMEG_GridManager;
enum class EMEGRoad:uint8;

UCLASS()
class MEGALO_CPP_API AMEG_RoadSpline : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AMEG_RoadSpline();
	 
	void MakeRoad(AMEG_GridCell* StartGridCell, AMEG_GridManager* GridManager);
	void MakeSingleRoad(AMEG_GridCell* _GridCell, EMEGRoad PreviousRoadEnd, AMEG_GridManager* _GridManager);
	FVector GetRoadSplinePointLocation(AMEG_GridCell* GridCell, EMEGRoad Road);

	TArray<FVector> SplinePointsLocation;

	bool isLoop = true;

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* SplineMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RemoveDuplicateLocations();

	bool isAlreadyCreated = true; // For the case where the roads has 2 ends (it will call the MakeRoad function 2 times)

	TArray<AMEG_GridCell*> VisitedCells;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
