// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/MEG_CellData.h"
#include "MEG_CardData.generated.h"

UENUM()
enum class EMEGCellPosition
{
	TL,
	TR,
	BL,
	BR
};

ENUM_RANGE_BY_FIRST_AND_LAST(EMEGCellPosition, EMEGCellPosition::TL, EMEGCellPosition::BR)

/**
 * 
 */
USTRUCT()
struct MEGALO_CPP_API FMEG_CardData
{
	GENERATED_BODY()
public:
	FMEG_CardData();
	~FMEG_CardData();

	UPROPERTY(EditDefaultsOnly)
	int32 CardID;

	UPROPERTY(EditDefaultsOnly)
	TMap<EMEGCellPosition, FMEG_CellData> Cells;
};
