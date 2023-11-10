// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/MEG_CellData.h"
#include "MEG_DistrictDataRow.generated.h"

/**
 * 
 */
USTRUCT()
struct MEGALO_CPP_API FMEG_DistrictDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	EMEGDistrict DistrictType;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* DistrictImage;
};
