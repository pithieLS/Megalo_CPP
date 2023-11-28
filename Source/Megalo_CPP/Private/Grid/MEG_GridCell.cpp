// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/MEG_GridCell.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
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

void AMEG_GridCell::UpdateCellWidget(EMEGDistrict DistrictType, TArray<EMEGRoad> _Roads)
{
	if (UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject()))
	{
		CastCellWidget->UpdateCell(DistrictType, _Roads);
		Roads = _Roads;
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

TArray<EMEGRoad> AMEG_GridCell::GetRoads() const
{
	if (const UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject()))
	{
		return CastCellWidget->Roads;
	}
	check(false);
	return TArray<EMEGRoad>();
}

void AMEG_GridCell::SetCellVisibilityAndOpacity(bool bVisibility, float _Opacity)
{
	const UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject());
	if (!ensure(CastCellWidget != nullptr))
		return;
	CellWidget->SetVisibility(bVisibility);
	CastCellWidget->GetDistrictImageComponent()->SetOpacity(_Opacity);
}

const UMEG_CellWidget* AMEG_GridCell::GetCellWidget() const
{
	const UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject());
	if (!ensure(CastCellWidget != nullptr))
		return nullptr;

	return CastCellWidget;
}

void AMEG_GridCell::SpawnMeshes()
{
	const TArray
}

//	Mesh->SetStaticMesh(StaticMeshes[FMath::FRand()*StaticMeshes.Num()];
