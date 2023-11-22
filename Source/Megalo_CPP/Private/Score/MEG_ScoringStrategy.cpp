// Fill out your copyright notice in the Description page of Project Settings.


#include "Score/MEG_ScoringStrategy.h"
#include "Grid/MEG_GridCell.h"

int32 UMEG_ScoreGoGreen::GetScore(const TArray<AMEG_GridCell*>& GridCells) const
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

int32 UMEG_ScoreBloomBloom::GetScore(const TArray<AMEG_GridCell*>& GridCells) const
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
