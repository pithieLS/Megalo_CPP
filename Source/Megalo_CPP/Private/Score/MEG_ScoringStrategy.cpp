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

	return Score;
}

int32 UMEG_ScoreMasterPlanned::GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const
{
	const AMEG_GridManager* GridManager = GameMode->GetGridManager();

	const int32 LargestIndustrialCluster = GridManager->GetBiggestDistrictClusterSize(EMEGDistrict::Industry);
	const int32 LargestDwellingCluster = GridManager->GetBiggestDistrictClusterSize(EMEGDistrict::Dwellings);

	return LargestDwellingCluster - LargestIndustrialCluster;
}

int32 UMEG_BlockParty::GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const
{
	int32 Score = 0;
	
	TArray<FVector2D> BlockNeighborOffset = { FVector2D(1, 0), FVector2D(0, 1), FVector2D(1, 1) };
	//TODO

	return Score;
}
