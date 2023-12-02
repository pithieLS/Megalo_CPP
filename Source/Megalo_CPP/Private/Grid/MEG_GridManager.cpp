// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/MEG_GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "MEG_GM.h"
#include "Data/MEG_CardData.h"
#include "Grid/MEG_GridCell.h"
#include "Grid/MEG_CardPlacer.h"
#include "UI/MEG_CardHand.h"
#include "UI/MEG_CardWidget.h"
#include "UI/MEG_CellWidget.h"
#include "UI/MEG_HUDWidget.h"

#define CELL_WIDTH 70
#define  CELL_HEIGHT 50
#define  DEFAULT_CELL_Z 1

// Sets default values
AMEG_GridManager::AMEG_GridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMEG_GridManager::MakeCardPreview(FVector2D _CardCoords)
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	UMEG_CardHand* CardHandWidget = GameMode->HUDWidget->WBP_Hand;
	if (!ensure(CardHandWidget != nullptr))
		return;

	const UMEG_CardWidget* CardWidget = CardHandWidget->GetSelectedCard();
	if (CardWidget == nullptr)
		return;

	const FMEG_CardData* CardData = GameMode->GetCardData(CardWidget->GetCardID());
	if (CardData == nullptr)
		return;

	// If card is rotated, change cell position and road direction
	for (const TPair<EMEGCellPosition, FMEG_CellData>& _CellData : CardData->Cells)
	{
		// If card is rotated, invert cell data
		TPair<EMEGCellPosition, FMEG_CellData> NewCellData;
		if (IsCardRotated)
			NewCellData = InvertCellData(_CellData);
		else
			NewCellData = _CellData;

		const FVector2D _CardOffsetCoords = _CardCoords + GetCellPositionOffset(NewCellData.Key);

		AMEG_GridCell* OveridenGridCell = GetCellFromCoords(_CardOffsetCoords);

		// If cell is already existing, hide it and add it to OverridenGridCells array
		if (OveridenGridCell != nullptr)
		{
			OveridenGridCell->SetCellVisibilityAndOpacity(false, 0.0f);
			OverridenGridCells.Add(OveridenGridCell);
		}

		// Create preview GridCell
		AMEG_GridCell* PreviewGridCell = GetWorld()->SpawnActor<AMEG_GridCell>(GridCellClassBP);
		GridCellPreviews.Add(PreviewGridCell);

		const FVector SpawnPosition = FVector(_CardOffsetCoords.X * CELL_WIDTH, _CardOffsetCoords.Y * CELL_HEIGHT, DEFAULT_CELL_Z);
		PreviewGridCell->SetActorLocation(SpawnPosition);
		PreviewGridCell->CellCoords = _CardOffsetCoords;
		PreviewGridCell->SetCellVisibilityAndOpacity(true, 0.5f);
		PreviewGridCell->UpdateCellWidget(_CellData.Value.DistrictType, NewCellData.Value.Roads);
	}

	PreviewCardCoords = _CardCoords;
}

void AMEG_GridManager::UndoCardPreview()
{
	for (AMEG_GridCell* _GridCellPreview : GridCellPreviews)
	{
		_GridCellPreview->Destroy(); //ConditionalBeginDestroy(); >>> trigger breakpoint
	}
	for (AMEG_GridCell* _OverridenGridCell : OverridenGridCells)
	{
		if (_OverridenGridCell != nullptr)
			_OverridenGridCell->SetCellVisibilityAndOpacity(true, 1.0f);
	}
	OverridenGridCells.Empty();
	GridCellPreviews.Empty();
}

void AMEG_GridManager::PlaceCard(int32 _CardID, FVector2D _CardCoords)
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	const FMEG_CardData* CardData = GameMode->GetCardData(_CardID);
	if (!ensure(CardData))
		return;

	AMEG_CardPlacer* CurrentPlacer = GetCardPlacerFromCoords(_CardCoords);
	if (CurrentPlacer != nullptr)
		CurrentPlacer->OnCardPlaced();

	for (const TPair<EMEGCellPosition, FMEG_CellData>& _CellData : CardData->Cells)
	{
		TPair<EMEGCellPosition, FMEG_CellData> NewCellData;
		if (IsCardRotated)
			NewCellData = InvertCellData(_CellData);
		else
			NewCellData = _CellData;

		const FVector2D _CardOffsetCoords = _CardCoords + GetCellPositionOffset(NewCellData.Key);

		AMEG_GridCell* CurrentGridCell = GetCellFromCoords(_CardOffsetCoords);
		if (CurrentGridCell == nullptr)
		{
			CurrentGridCell = GetWorld()->SpawnActor<AMEG_GridCell>(GridCellClassBP);
			if (!ensure(CurrentGridCell != nullptr))
				continue;

			GridCells.Add(CurrentGridCell);

			const FVector SpawnPosition = FVector(_CardOffsetCoords.X * CELL_WIDTH, _CardOffsetCoords.Y * CELL_HEIGHT, DEFAULT_CELL_Z);
			CurrentGridCell->SetActorLocation(SpawnPosition);
			CurrentGridCell->CellCoords = _CardOffsetCoords;
		}
		if (!ensure(CurrentGridCell != nullptr))
			continue;
		CurrentGridCell->UpdateCellWidget(_CellData.Value.DistrictType, NewCellData.Value.Roads);
	}
	UpdateCardPlacers(_CardCoords);
	UndoCardPreview();
	IsCardRotated = false;
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

void AMEG_GridManager::RotateCard()
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	if (GameMode->HUDWidget->WBP_Hand->GetSelectedCard() == nullptr)
		return;

	IsCardRotated = !IsCardRotated;

	UndoCardPreview();
	MakeCardPreview(PreviewCardCoords);
}

TPair<EMEGCellPosition, FMEG_CellData> AMEG_GridManager::InvertCellData(TPair<EMEGCellPosition, FMEG_CellData> _CellData) const
{
	TPair<EMEGCellPosition, FMEG_CellData> NewCardData;

	// Invert cell offset
	switch (_CellData.Key)
	{
	case EMEGCellPosition::TL:
		NewCardData.Key = EMEGCellPosition::BR;
		break;
	case EMEGCellPosition::TR:
		NewCardData.Key = EMEGCellPosition::BL;
		break;
	case EMEGCellPosition::BL:
		NewCardData.Key = EMEGCellPosition::TR;
		break;
	case EMEGCellPosition::BR:
		NewCardData.Key = EMEGCellPosition::TL;
		break;
	default:
		break;
	}

	// Invert roads
	for (int32 Index = 0; Index < _CellData.Value.Roads.Num(); Index++)
	{
		NewCardData.Value.Roads.Add(GetOppositeRoad(_CellData.Value.Roads[Index]));
	}

	return NewCardData;
}

// Called when the game starts or when spawned
void AMEG_GridManager::BeginPlay()
{
	Super::BeginPlay();

	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	GameMode->OnRotatePressedDelegate.BindUObject(this, &AMEG_GridManager::RotateCard);
}

void AMEG_GridManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	GameMode->OnRotatePressedDelegate.Unbind();
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

int32 AMEG_GridManager::GetRoadCount() const
{
	TArray<FVector2D> VisitedCoords;
	int32 NumRoads = 0;

	for (const AMEG_GridCell* _GridCell : GridCells)
	{
		if (!ensure(_GridCell != nullptr))
			continue;

		if (_GridCell->GetRoads().Num() == 0)
			continue; // No roads

		// Skip it if already visited
		if (VisitedCoords.Contains(_GridCell->CellCoords))
			continue;

		NumRoads++;

		// No need to know the length of the road, we just want to mark this entire road as visited
		VisitSingleRoad(_GridCell, VisitedCoords);
	}
	return NumRoads;
}

TArray<class AMEG_GridCell*> AMEG_GridManager::GetGridCells() const
{
	return GridCells;
}

TArray<AMEG_GridCell*> AMEG_GridManager::GetAllCellsFromAxis(int32 AxisValue)
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return{};


	const TArray<AMEG_GridCell*> AxisCells = GetGridCells().FilterByPredicate([this, AxisValue](const AMEG_GridCell* _AxisCell)
		{
			if (AxisValue == 0)
			{
				if (_AxisCell->CellCoords.X == AxisValue)
					return true;
			}
			else
			{
				if (_AxisCell->CellCoords.Y == AxisValue)
					return true;
			}
		});

	return AxisCells;
}

void AMEG_GridManager::VisitSingleRoad(const AMEG_GridCell* _GridCell, TArray<FVector2D>& VisitedCoords) const
{
	if (_GridCell->GetRoads().Num() == 0)
		return; // No roads

	if (VisitedCoords.Contains(_GridCell->CellCoords))
		return;

	VisitedCoords.Add(_GridCell->CellCoords);

	// Check every road direction to see if it's connected on a neighbor cell
	for (const EMEGRoad Road : _GridCell->GetRoads())
	{
		const FVector2D Offset = GetRoadNeighborOffset(Road);
		const AMEG_GridCell* NeighBorCell = GetCellFromCoords(_GridCell->CellCoords + Offset);

		if (NeighBorCell == nullptr) // There are no cell at this coord
			continue;

		if (!NeighBorCell->GetRoads().Contains(GetOppositeRoad(Road)))
			continue;

		VisitSingleRoad(NeighBorCell, VisitedCoords);
	}
}

const FVector2D AMEG_GridManager::GetRoadNeighborOffset(const EMEGRoad& Road) const
{
	switch (Road)
	{
	case EMEGRoad::Up:
		return FVector2D(0, -1);
	case EMEGRoad::Down:
		return FVector2D(0, 1);
	case EMEGRoad::Left:
		return FVector2D(-1, 0);
	case EMEGRoad::Right:
		return FVector2D(0, 1);
	default:
		return FVector2D(0, 0);
	}
}
const EMEGRoad AMEG_GridManager::GetOppositeRoad(const EMEGRoad InitialDirection) const
{
	int32 IntDirection = (int32)InitialDirection;
	IntDirection = (IntDirection + 2) % 4;
	const EMEGRoad OppositeDirection = (EMEGRoad)IntDirection;
	return OppositeDirection;
}
