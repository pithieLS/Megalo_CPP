// Fill out your copyright notice in the Description page of Project Settings.


#include "MEG_RoadSpline.h"
#include "Components/SplineComponent.h"
#include "Data/MEG_CellData.h"
#include "Grid/MEG_GridCell.h"
#include "Grid/MEG_GridManager.h"

// Sets default values
AMEG_RoadSpline::AMEG_RoadSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if (!ensure(SplineComponent != nullptr))
		return;
	SetRootComponent(SplineComponent);
}

void AMEG_RoadSpline::MakeRoad(AMEG_GridCell* StartGridCell, AMEG_GridManager* GridManager)
{
	if (!isAlreadyCreated)
		return;
	isAlreadyCreated = false;

	TArray<EMEGRoad> StartCellRoads = StartGridCell->GetRoads();
	if(!isLoop)
		for (EMEGRoad _Road : StartCellRoads)
		{
			const FVector2D Offset = GridManager->GetRoadNeighborOffset(_Road);
			AMEG_GridCell* NeighBorCell = GridManager->GetCellFromCoords(StartGridCell->CellCoords + Offset);

			if (NeighBorCell == nullptr) // If road lead to the end of the road
			{
				FVector SplineStartLocation = GetRoadSplinePointLocation(StartGridCell, _Road);
				SplineComponent->SetLocationAtSplinePoint(0, SplineStartLocation, ESplineCoordinateSpace::World); // Set location of the first spline point
			
				continue;
			}

			MakeSingleRoad(StartGridCell, _Road, GridManager); // If road don't lead to the end, start making spline
		}
	else
		MakeSingleRoad(StartGridCell, StartCellRoads[0], GridManager); // If is loop, start with the first cell road

	for (FVector PointLocation : SplinePointsLocation) // Add Spline point at each recorded locations
	{
		SplineComponent->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
	}

	if (isLoop) // If the road is a loop, set the first spline point location to the last spline point location so it loop
	{
		FVector SplineStartLocation = SplinePointsLocation[SplinePointsLocation.Num() - 1];
		SplineComponent->SetLocationAtSplinePoint(0, SplineStartLocation, ESplineCoordinateSpace::World);
	}

}

void AMEG_RoadSpline::MakeSingleRoad(AMEG_GridCell* _GridCell, EMEGRoad PreviousRoadEnd, AMEG_GridManager* _GridManager)
{
	TArray<EMEGRoad> CellRoads = _GridCell->GetRoads();
	if (CellRoads.Num() == 0)
		return;

	FVector NewSplinePointPos = _GridCell->SplinePointMiddle->GetComponentLocation();
	//if (SplinePointsLocation.Contains(NewSplinePointPos))
	//	return;

	VisitedCells.Add(_GridCell);

	SplinePointsLocation.Add(NewSplinePointPos); // Add middle point

	EMEGRoad OppositeRoad = _GridManager->GetOppositeRoad(PreviousRoadEnd);
	FVector2D Offset;
	EMEGRoad LinkedRoad; // 
	for (EMEGRoad _Road : _GridCell->GetRoads()) // If the road isn't linked to the previous road, add point (because the old road point is already added)
	{
		if(_Road == OppositeRoad)
			continue;

		SplinePointsLocation.Add(GetRoadSplinePointLocation(_GridCell, _Road));
		Offset = _GridManager->GetRoadNeighborOffset(_Road);
		LinkedRoad = _Road;
	}

	AMEG_GridCell* NeighBorCell = _GridManager->GetCellFromCoords(_GridCell->CellCoords + Offset);
	if (NeighBorCell == nullptr)
		return;

	if (VisitedCells.Contains(NeighBorCell))
		return;


	MakeSingleRoad(NeighBorCell, LinkedRoad, _GridManager);
}

FVector AMEG_RoadSpline::GetRoadSplinePointLocation(AMEG_GridCell* GridCell, EMEGRoad Road)
{
	FVector PointLocation;

	switch ((uint8)Road)
	{
	case 0:
		PointLocation = GridCell->SplinePointUp->GetComponentLocation();
		break;
	case 1:
		PointLocation = GridCell->SplinePointRight->GetComponentLocation();
		break;
	case 2:
		PointLocation = GridCell->SplinePointDown->GetComponentLocation();
		break;
	case 3:
		PointLocation = GridCell->SplinePointLeft->GetComponentLocation();
		break;
	default:
		break;
	}

	return PointLocation;
}

// Called when the game starts or when spawned
void AMEG_RoadSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMEG_RoadSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

