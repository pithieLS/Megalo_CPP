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

	UFUNCTION(BlueprintCallable)
	void MakeCardPreview(FVector2D _CardCoords);

	UFUNCTION(BlueprintCallable)
	void UndoCardPreview();

	void PlaceCard(int32 _CardID, FVector2D _CardCoords);
	int32 GetBiggestDistrictClusterSize(const EMEGDistrict DistrictType) const;
	int32 GetDistrictClusterSize(const AMEG_GridCell* GridCell, const EMEGDistrict DistrictType, TArray<FVector2D>& VisitedCoords) const;
	int32 GetRoadCount() const;
	TArray<AMEG_GridCell*> GetGridCells() const;
	TArray<AMEG_GridCell*> GetAllCellsFromAxis(int32 AxisValue); // 0 = Row, 1 = Column
	AMEG_GridCell* GetCellFromCoords(FVector2D _OffsetCoords) const;
	AMEG_CardPlacer* GetCardPlacerFromCoords(FVector2D _Coords) const;
	void RotateCard();
	TPair<enum EMEGCellPosition, struct FMEG_CellData> InvertCellData(TPair<EMEGCellPosition, FMEG_CellData> _CellData) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	const FVector2D GetCellPositionOffset(EMEGCellPosition _CellPosition) const;
	void UpdateCardPlacers(FVector2D _Coords);
	void VisitSingleRoad(const AMEG_GridCell* InGridCell, TArray<FVector2D>& VisitedCoords) const;
	const FVector2D GetRoadNeighborOffset(const EMEGRoad& Road) const;
	const EMEGRoad GetOppositeRoad(const EMEGRoad InitialDirection) const;


	bool IsCardRotated = false;

	TArray<class AMEG_GridCell*> GridCells;
	TArray<AMEG_CardPlacer*> CardPlacers;
	TArray<FVector2D> NeighborsOffset = { FVector2D(-1,0), FVector2D(1,0),FVector2D(0,-1), FVector2D(0,1) };

	// Card Preview Related
	TArray<AMEG_GridCell*> OverridenGridCells;
	TArray<AMEG_GridCell*> GridCellPreviews;
	FVector2D PreviewCardCoords;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMEG_CardPlacer> CardPlacerClassBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AMEG_GridCell> GridCellClassBP;



public:	

};
