// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEG_CellWidget.h"
#include "Data/MEG_DistrictDataRow.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/Overlay.h"
#include "MEG_GM.h"

void UMEG_CellWidget::UpdateCell(EMEGDistrict _DistrictType, TArray<EMEGRoad> _Roads)
{
	UpdateDistrict(_DistrictType);
	UpdateRoads(_Roads);
}

UImage* UMEG_CellWidget::GetDistrictImageComponent() const
{
	return DistrictImage;
}

void UMEG_CellWidget::UpdateDistrict(EMEGDistrict _DistrictType)
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	const FMEG_DistrictDataRow* _DistrictDataRowBuffer = GameMode->GetDistrictDataRow(_DistrictType);
	if (!ensure(_DistrictDataRowBuffer))
		return;

	UTexture2D* _DistrictTexture = GameMode->GetDistrictDataRow(_DistrictType)->DistrictImage;

	DistrictImage->SetBrushFromTexture(_DistrictTexture);

	DistrictType = _DistrictType;
}

void UMEG_CellWidget::UpdateRoads(TArray<EMEGRoad> _Roads)
{
	Roads = _Roads;

	if (Roads.Contains(EMEGRoad::Up))
		UpBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		UpBox->SetVisibility(ESlateVisibility::Collapsed);
	if (Roads.Contains(EMEGRoad::Right))
		RightBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		RightBox->SetVisibility(ESlateVisibility::Collapsed);
	if (Roads.Contains(EMEGRoad::Down))
		DownBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		DownBox->SetVisibility(ESlateVisibility::Collapsed);
	if (Roads.Contains(EMEGRoad::Left))
		LeftBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		LeftBox->SetVisibility(ESlateVisibility::Collapsed);
}
