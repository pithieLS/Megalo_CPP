// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MEG_CardData.h"

FMEG_CardData::FMEG_CardData()
{
	for (EMEGCellPosition Position : TEnumRange<EMEGCellPosition>())
	{
		Cells.Add({Position, FMEG_CellData()});
	}
}

FMEG_CardData::~FMEG_CardData()
{

}
