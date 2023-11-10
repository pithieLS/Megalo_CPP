// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEG_CellWidget.h"
#include "Data/MEG_DistrictDataRow.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "MEG_GM.h"

void UMEG_CellWidget::UpdateCell(EMEGDistrict InDistrictType)
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	const FMEG_DistrictDataRow* _DistrictDataRowBuffer = GameMode->GetDistrictDataRow(InDistrictType);
	if (!ensure(_DistrictDataRowBuffer))
		return;

	UTexture2D* _DistrictTexture = GameMode->GetDistrictDataRow(InDistrictType)->DistrictImage;

	DistrictImage->SetBrushFromTexture(_DistrictTexture);
}
