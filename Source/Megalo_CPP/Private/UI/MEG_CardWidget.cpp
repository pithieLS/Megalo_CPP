// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEG_CardWidget.h"
#include "UI/MEG_CellWidget.h"
#include "MEG_GM.h"
#include "Data/MEG_CardData.h"
#include "Kismet/GameplayStatics.h"

void UMEG_CardWidget::NativeConstruct()
{
	FillCellWidgetMap();
}

void UMEG_CardWidget::UpdateCard(int32 CardID)
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	for (TPair<EMEGCellPosition, UMEG_CellWidget*> cellPair : CellWidgets)
	{
		FMEG_CardData cardData = GameMode->Cards[CardID];
		cellPair.Value->UpdateCell(cardData.Cells[cellPair.Key].DistrictType);
	}
}

void UMEG_CardWidget::FillCellWidgetMap()
{
	CellWidgets.Add({ EMEGCellPosition::TL, CellTL });
	CellWidgets.Add({ EMEGCellPosition::TR, CellTR });
	CellWidgets.Add({ EMEGCellPosition::BL, CellBL });
	CellWidgets.Add({ EMEGCellPosition::BR, CellBR });
}
