// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEG_CardWidget.h"
#include "UI/MEG_CellWidget.h"
#include "MEG_GM.h"
#include "Data/MEG_CardData.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UMEG_CardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FillCellWidgetMap();
	
	CardButton->OnClicked.AddDynamic(this, &UMEG_CardWidget::OnButtonClicked);
}

void UMEG_CardWidget::NativeDestruct()
{
	Super::NativeDestruct();

	CardButton->OnClicked.RemoveAll(this);
}

int32 UMEG_CardWidget::GetCardID()
{
	return CardID;
}

bool UMEG_CardWidget::GetIsSelected()
{
	return IsCardSelected;
}

void UMEG_CardWidget::SetSelected(bool _IsSelected)
{
	IsCardSelected = _IsSelected;
	if (IsCardSelected)
		SelectedImage->SetVisibility(ESlateVisibility::Visible);
	else
		SelectedImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UMEG_CardWidget::UpdateCard(int32 _CardID)
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	const FMEG_CardData* CardData = GameMode->GetCardData(_CardID);
	if (!ensure(CardData))
		return;

	CardID = CardData->CardID;

	///
	for (TPair<EMEGCellPosition, UMEG_CellWidget*>& cellPair : CellWidgets)
	{
		if (!ensure(cellPair.Value))
			return;

		const EMEGDistrict CellDistrict = CardData->Cells[cellPair.Key].DistrictType;

		cellPair.Value->UpdateCell(CellDistrict);
	}
}

void UMEG_CardWidget::FillCellWidgetMap()
{
	CellWidgets.Add({ EMEGCellPosition::TL, CellTL });
	CellWidgets.Add({ EMEGCellPosition::TR, CellTR });
	CellWidgets.Add({ EMEGCellPosition::BL, CellBL });
	CellWidgets.Add({ EMEGCellPosition::BR, CellBR });
}

void UMEG_CardWidget::OnButtonClicked()
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	GameMode->OnCardSelectedDelegate.Execute(CardID);

}
