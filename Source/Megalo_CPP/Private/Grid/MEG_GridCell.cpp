// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/MEG_GridCell.h"
#include "Components/WidgetComponent.h"
#include "Data/MEG_CardData.h"
#include "UI/MEG_CellWidget.h"
#include "UI/MEG_CellWidget.h"
#include "Data/MEG_CellData.h"

// Sets default values
AMEG_GridCell::AMEG_GridCell()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	CellWidget = CreateDefaultSubobject<UWidgetComponent>("CellWidget");
	CellWidget->SetupAttachment(RootComponent);
}

void AMEG_GridCell::UpdateCellDistrict(EMEGDistrict DistrictType)
{
	if (UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject()))
	{
		CastCellWidget->UpdateCell(DistrictType);
	}
}

EMEGDistrict AMEG_GridCell::GetDistrictType() const
{
	if (const UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject()))
	{
		return CastCellWidget->DistrictType;
	}
	check(false);
	return EMEGDistrict::Parc;
}
