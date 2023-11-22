// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MEG_GridManager.generated.h"

enum class EMEGCellPosition;
enum class EMEGDistrict;
enum class EMEGRoad:uint8;
class AMEG_GridCell;
class AMEG_CardPlacer;

UCLASS()
class MEGALO_CPP_API AMEG_GridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMEG_GridManager();

	void PlaceCard(int32 _CardID, FVector2D _CardCoords);
	int32 GetBiggestDistrictClusterSize(const EMEGDistrict DistrictType) const;
	int32 GetDistrictClusterSize(const AMEG_GridCell* GridCell, const EMEGDistrict DistrictType, TArray<FVector2D>& VisitedCoords) const;
	int32 GetRoadCount() const;
	TArray<class AMEG_GridCell*> GetGridCells() const;
	AMEG_GridCell* GetCellFromCoords(FVector2D _OffsetCoords) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const FVector2D GetCellPositionOffset(EMEGCellPosition _CellPosition) const;
	void UpdateCardPlacers(FVector2D _Coords);
	void VisitSingleRoad(const AMEG_GridCell* InGridCell, TArray<FVector2D>& VisitedCoords) const;
	const FVector2D GetRoadNeighborOffset(const EMEGRoad& Road) const;
	const EMEGRoad GetOppositeRoad(const EMEGRoad InitialDirection) const;

	TArray<class AMEG_GridCell*> GridCells;
	TArray<AMEG_CardPlacer*> CardPlacers;
	AMEG_CardPlacer* GetCardPlacerFromCoords(FVector2D _Coords) const;
	
	TArray<FVector2D> NeighborsOffset = { FVector2D(-1,0), FVector2D(1,0),FVector2D(0,-1), FVector2D(0,1) };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMEG_CardPlacer> CardPlacerClassBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AMEG_GridCell> GridCellClassBP;

public:	

};
