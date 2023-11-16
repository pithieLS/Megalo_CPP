// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/MEG_GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "MEG_GM.h"
#include "Data/MEG_CardData.h"
#include "Grid/MEG_GridCell.h"
#include "Grid/MEG_CardPlacer.h"

#define CELL_WIDTH 70
#define  CELL_HEIGHT 50
#define  DEFAULT_CELL_Z 1

// Sets default values
AMEG_GridManager::AMEG_GridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMEG_GridManager::PlaceCard(int32 _CardID, FVector2D _CardCoords)
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	const FMEG_CardData* CardData = GameMode->GetCardData(_CardID);
	if (!ensure(CardData))
		return;

	for (const TPair<EMEGCellPosition, FMEG_CellData>& _CellData : CardData->Cells)
	{
		const FVector2D OffsetCoords = _CardCoords + GetCellPositionOffset(_CellData.Key);

		AMEG_GridCell* CurrentGridCell = GetCellFromCoords(OffsetCoords);
		if (CurrentGridCell == nullptr)
		{
			CurrentGridCell = GetWorld()->SpawnActor<AMEG_GridCell>(GridCellClassBP);
			if (!ensure(CurrentGridCell != nullptr))
				continue;

			GridCells.Add(CurrentGridCell);

			const FVector SpawnPosition = FVector(OffsetCoords.X * CELL_WIDTH, OffsetCoords.Y * CELL_HEIGHT, DEFAULT_CELL_Z);
			CurrentGridCell->SetActorLocation(SpawnPosition);
			CurrentGridCell->CellCoords = OffsetCoords;
		}
		if (!ensure(CurrentGridCell != nullptr))
			continue;
		CurrentGridCell->UpdateCellDistrict(_CellData.Value.DistrictType);
	}
	UpdateCardPlacers(_CardCoords);
}

void AMEG_GridManager::UpdateCardPlacers(FVector2D _Coords)
{
	for (int32 XOffset = -2; XOffset <= 2; XOffset++)
	{
		for (int32 YOffset = -2; YOffset <= 2; YOffset++)
		{
			if (abs(XOffset) + abs(YOffset) == 4)
				continue;

			const int32 CurrentXCoord = _Coords.X + XOffset;
			const int32 CurrentYCoord = _Coords.Y + YOffset;

			AMEG_CardPlacer* CurrentPlacer = GetCardPlacerFromCoords(FVector2D(CurrentXCoord, CurrentYCoord));

			if (CurrentPlacer == nullptr) // If placer not found, create it
			{
				CurrentPlacer = GetWorld()->SpawnActor<AMEG_CardPlacer>(CardPlacerClassBP);
				if (!ensure(CurrentPlacer != nullptr))
					continue;

				CurrentPlacer->Coords = FVector2D(CurrentXCoord, CurrentYCoord);
				const FVector CurrentCoords = FVector(CurrentXCoord * CELL_WIDTH, CurrentYCoord * CELL_HEIGHT, DEFAULT_CELL_Z);

				CurrentPlacer->SetActorLocation(CurrentCoords);
				CardPlacers.Add(CurrentPlacer);
			}
		}
	}
}

AMEG_CardPlacer* AMEG_GridManager::GetCardPlacerFromCoords(FVector2D _Coords) const
{
	AMEG_CardPlacer* const* Placer = CardPlacers.FindByPredicate([_Coords](const AMEG_CardPlacer* _Placer)
		{
			return _Placer->Coords == _Coords;
		});
	if (Placer == nullptr)
		return nullptr;
	return *Placer;
}

// Called when the game starts or when spawned
void AMEG_GridManager::BeginPlay()
{
	Super::BeginPlay();
	
}

const FVector2D AMEG_GridManager::GetCellPositionOffset(EMEGCellPosition _CellPosition) const
{
	switch (_CellPosition)
	{
	case EMEGCellPosition::TL:
		return FVector2D(-1, -1);
		break;
	case EMEGCellPosition::TR:
		return FVector2D(0, -1);
		break;
	case EMEGCellPosition::BL:
		return FVector2D(-1, 0);
		break;
	case EMEGCellPosition::BR:
		return FVector2D(0, 0);
		break;
	default:
		return FVector2D(0, 0);
		break;
	}
}

int32 AMEG_GridManager::GetBiggestDistrictClusterSize(const EMEGDistrict _DistrictType) const
{
	TArray<FVector2D> VisitedCoords;
	int32 BiggestClusterSize = 0;

	for (const AMEG_GridCell* GridCell : GridCells)
	{
		if (!ensure(GridCell != nullptr))
			continue;

		if (GridCell->GetDistrictType() != _DistrictType)
			continue;

		// Skip it if already visited
		if (VisitedCoords.Contains(GridCell->CellCoords))
			continue;

		const int32 ClusterSize = GetDistrictClusterSize(GridCell, GridCell->GetDistrictType(), VisitedCoords);
		BiggestClusterSize = ClusterSize > BiggestClusterSize ? ClusterSize : BiggestClusterSize;
	}
	return BiggestClusterSize;
}

int32 AMEG_GridManager::GetDistrictClusterSize(const AMEG_GridCell* _GridCell, const EMEGDistrict District, TArray<FVector2D>& VisitedCoords) const
{
	if (_GridCell->GetDistrictType() != District)
		return 0;

	// If cell already visited, return
	if (VisitedCoords.Contains(_GridCell->CellCoords))
		return 0;

	VisitedCoords.Add(_GridCell->CellCoords);

	int32 Size = 1;

	// Call the function recursively on all 4 neighbors (if they exist)
	for (const FVector2D NeighBorCoords : NeighborsOffset)
	{
		const FVector2D NeighBorCellGridCoords = _GridCell->CellCoords + NeighBorCoords;
		const AMEG_GridCell* NeighBorCell = GetCellFromCoords(_GridCell->CellCoords + NeighBorCoords);
		if (NeighBorCell == nullptr) // There are no cell at this coord
			continue;
		Size += GetDistrictClusterSize(NeighBorCell, District, VisitedCoords);
	}
	return Size;
}


AMEG_GridCell* AMEG_GridManager::GetCellFromCoords(FVector2D _OffsetCoords) const
{
	AMEG_GridCell*const* _GridCell = GridCells.FindByPredicate([_OffsetCoords](const AMEG_GridCell* _GridCell)
		{
			return _GridCell->CellCoords == _OffsetCoords;
		});
	if (_GridCell == nullptr)
		return nullptr;
	return *_GridCell;
}

