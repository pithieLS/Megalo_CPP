// Fill out your copyright notice in the Description page of Project Settings.


#include "Score/MEG_ScoringStrategy.h"
#include "Kismet/GameplayStatics.h"
#include "Grid/MEG_GridCell.h"
#include "Grid/MEG_GridManager.h"
#include "MEG_GM.h"

int32 UMEG_ScoreGoGreen::GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const
{
	int32 Score = 0;
	for (const AMEG_GridCell* _GridCell : GridCells)
	{
		switch (_GridCell->GetDistrictType())
		{
		case EMEGDistrict::Industry:
			Score -= 3;
			break;
		case EMEGDistrict::Parc:
			Score += 1;
			break;
		default:
			break;
		}
	}
	//return Score;
	return 0;
}

int32 UMEG_ScoreBloomBloom::GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const
{
	int32 Score = 0;
	TArray<float> VisitedYCoords;
	TArray<float> VisitedXCoords;

	//Iterating through columns  
	for (const AMEG_GridCell* _GridCell : GridCells)
	{
		if(VisitedYCoords.Contains(_GridCell->CellCoords.Y))
			continue;

		VisitedYCoords.Add(_GridCell->CellCoords.Y);

		float currentY = _GridCell->CellCoords.Y;

		const TArray<AMEG_GridCell*> ColumnCells = GridCells.FilterByPredicate([this, currentY](const AMEG_GridCell* _ColumnCell)
			{
				return _ColumnCell->CellCoords.Y == currentY;
			});

		int ParcCellYNb = 0;
		for (AMEG_GridCell* _cell : ColumnCells)
		{
			if (_cell->GetDistrictType() == EMEGDistrict::Parc)
				ParcCellYNb++;
		}

		if (ParcCellYNb == 3)
			Score++;
		else if (ParcCellYNb == 0)
			Score--;
	}

	//Iterating through rows 
	for (const AMEG_GridCell* _GridCell : GridCells)
	{
		if (VisitedXCoords.Contains(_GridCell->CellCoords.X))
			continue;

		VisitedXCoords.Add(_GridCell->CellCoords.X);

		float currentX = _GridCell->CellCoords.X;

		const TArray<AMEG_GridCell*> ColumnCells = GridCells.FilterByPredicate([this, currentX](const AMEG_GridCell* _ColumnCell)
			{
				return _ColumnCell->CellCoords.X == currentX;
			});

		int ParcCellXNb = 0;
		for (AMEG_GridCell* _cell : ColumnCells)
		{
			if (_cell->GetDistrictType() == EMEGDistrict::Parc)
				ParcCellXNb++;
		}

		if (ParcCellXNb == 3)
			Score++;
		else if (ParcCellXNb == 0)
			Score--;
	}

	//return Score;
	return 0;
}

int32 UMEG_ScoreStackAndScrapers::GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const
{
	int32 Score = 0;

	const AMEG_GridManager* GridManager = GameMode->GetGridManager();

	// Get all the industry Cells
	const TArray<AMEG_GridCell*> IndustryCells = GridCells.FilterByPredicate([this, GridCells](const AMEG_GridCell* _GridCell)
		{
			return _GridCell->GetDistrictType() == EMEGDistrict::Industry;
		});

	const TArray<FVector2D> NeighbourOffsets = {FVector2D(0, -1), FVector2D(1, 0),
										  FVector2D(0, 1), FVector2D(-1, 0)};
	for (const AMEG_GridCell* _GridCell : IndustryCells)
	{
		bool isOnlyIndusCom = true;

		// For each neighbor cells, check if any isn't industry or commercial. 
		// If not industry or commercial check for next industry cell, else, add 2 to score.
		const FVector2D CellCoords = _GridCell->CellCoords;
		for (FVector2D Offset : NeighbourOffsets)
		{
			const AMEG_GridCell* NeighbourCell = GridManager->GetCellFromCoords(CellCoords + Offset);
			if (NeighbourCell == nullptr)
				continue;

			const EMEGDistrict NeighbourDistrictType = NeighbourCell->GetDistrictType();
			if (NeighbourDistrictType != EMEGDistrict::Industry && NeighbourDistrictType != EMEGDistrict::Commercial)
			{
				isOnlyIndusCom = false;
				break;
			}
		}
		if (isOnlyIndusCom)
				Score += 2;
	}

	//return Score;
	return 0;
}

int32 UMEG_ScoreMasterPlanned::GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const
{
	const AMEG_GridManager* GridManager = GameMode->GetGridManager();

	const int32 LargestIndustrialCluster = GridManager->GetBiggestDistrictClusterSize(EMEGDistrict::Industry);
	const int32 LargestDwellingCluster = GridManager->GetBiggestDistrictClusterSize(EMEGDistrict::Dwellings);

	//return LargestDwellingCluster - LargestIndustrialCluster;
	return 0;
}

int32 UMEG_BlockParty::GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const
{
	int32 GroupNb = 0;

	const TArray<FVector2D> BlockNeighborOffset = { FVector2D(1, 0), FVector2D(0, 1), FVector2D(1, 1) };
	const AMEG_GridManager* GridManager = GameMode->GetGridManager();
	if (!ensure(GridManager != nullptr))
		return 0;
	
	// For each _GridCell, check neighbor's district type (R, D, DR)
	// If one NeighborCell != _GridCell's DistrictType: check next _GridCell
	// Else, GroupNb++
	for (AMEG_GridCell* _GridCell : GridCells)
	{
		const EMEGDistrict CellDistrictType = _GridCell->GetDistrictType();
		const FVector2D CellCoords = _GridCell->CellCoords;
		bool isBlock = true;

		//Check neighbors with offset
		for (FVector2D _Offset : BlockNeighborOffset)
		{
			const FVector2D NeighbourCoords = CellCoords + _Offset;
			const AMEG_GridCell* NeighborCell =  GridManager->GetCellFromCoords(NeighbourCoords);
			if (NeighborCell == nullptr || NeighborCell->GetDistrictType() != CellDistrictType)
			{
				isBlock = false;
				break;
			}
		}
		if (isBlock)
			GroupNb++;
	}

	//// Didn't used a switch because it don't have a condition case
	//if (GroupNb == 0)
	//	return -8;
	//else if (GroupNb == 1)
	//	return -5;
	//else if (GroupNb == 2)
	//	return -2;
	//else if (GroupNb == 3)
	//	return 1;
	//else if (GroupNb == 4)
	//	return 4;
	//else if (GroupNb >= 5)
	//	return 7;

	//return 0;

	return 0;
}


//int32 UMEG_TheOutskirsts::GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const
//{
//	int32 Score = 0;
//
//	TMap<AMEG_GridCell*, TArray<FVector2D>> EmptyNeighborCells;
//	TMap<FVector2D, uint8> NeighbourOffsetsDirection = {{FVector2D(0, -1), 0},
//													{FVector2D(1, 0), 1},
//													{FVector2D(0, 1), 2},
//													{FVector2D(-1, 0), 3}};
//
//	for (AMEG_GridCell* _GridCell : GridCells)
//	{
//		TArray<FVector2D> EmptyNeighborOffsets;
//
//		for (TPair<FVector2D, uint8> _NeighbourOffsetDirection : NeighbourOffsetsDirection)
//		{
//			if (GameMode->GetGridManager()->GetCellFromCoords(_NeighbourOffsetDirection.Key) == nullptr)
//			{
//				EmptyNeighborOffsets.Add(_NeighbourOffsetDirection.Key);
//			}
//		}
//		EmptyNeighborCells.Add(_GridCell, EmptyNeighborOffsets);
//	}
//
//	for (TPair<AMEG_GridCell*, TArray<FVector2D>> _EmptyNeighborOffsetCell : EmptyNeighborCells)
//	{
//		TArray<EMEGRoad> Roads = _EmptyNeighborOffsetCell.Key->Roads;
//		bool isRoadEndAtEdge = false;
//
//		for (EMEGRoad _Road : Roads)
//		{
//			for (FVector2D _EmptyNeighborOffset : _EmptyNeighborOffsetCell.Value)
//			{
//				if (NeighbourOffsetsDirection.Find(_EmptyNeighborOffset) != nullptr)
//					if (*NeighbourOffsetsDirection.Find(_EmptyNeighborOffset) == (uint8)_Road)
//					{
//						Score--;
//						isRoadEndAtEdge = true;
//						break;
//					}
//			}
//			if (isRoadEndAtEdge)
//				break;
//		}
//		if (!isRoadEndAtEdge)
//			Score++;
//
//	}
//
//	return Score;
//}

int32 UMEG_CentralPerks::GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const
{
	int32 Score = 0;

	const TArray<FVector2D> NeighbourOffsets = { FVector2D(0, -1), FVector2D(1, 0),
												FVector2D(0, 1), FVector2D(-1, 0) };

	const TArray<AMEG_GridCell*> InCityParcCells = GridCells.FilterByPredicate([this, NeighbourOffsets, GameMode](const AMEG_GridCell* _GridCell)
		{
			bool isParkCell = false;
			bool isInCity = true;

			// Check if it is the cell is a Parc district
			if (_GridCell->GetDistrictType() != EMEGDistrict::Parc)
				return false;

			// If so, check if cell has any empty neighbor
			for (FVector2D _Offset : NeighbourOffsets)
			{
				FVector2D NeighborCoords = _GridCell->CellCoords + _Offset;
				if (GameMode->GetGridManager()->GetCellFromCoords(NeighborCoords) == nullptr)
				{
					isInCity = false;
					break;
				}
			}

			return isInCity;
		});

	const TArray<AMEG_GridCell*> OutCityParcCells = GridCells.FilterByPredicate([this, NeighbourOffsets, GameMode](const AMEG_GridCell* _GridCell)
		{
			bool isParkCell = false;
			bool isInCity = true;

			// Check if it is the cell is a Parc district
			if (_GridCell->GetDistrictType() != EMEGDistrict::Parc)
				return false;

			// If so, check if cell has any empty neighbor
			for (FVector2D _Offset : NeighbourOffsets)
			{
				FVector2D NeighborCoords = _GridCell->CellCoords + _Offset;
				if (GameMode->GetGridManager()->GetCellFromCoords(NeighborCoords) == nullptr)
				{
					isInCity = false;
					break;
				}
			}

			return !isInCity;
		});

	Score += InCityParcCells.Num();
	Score -= OutCityParcCells.Num() * 2;

	//return Score;
	return 0;
}
