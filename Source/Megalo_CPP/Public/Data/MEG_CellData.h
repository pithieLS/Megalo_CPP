// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MEG_CellData.generated.h"

UENUM()
enum class EMEGDistrict
{
	Parc,
	Commercial,
	Industry,
	Dwellings
};
ENUM_RANGE_BY_FIRST_AND_LAST(EMEGDistrict, EMEGDistrict::Parc, EMEGDistrict::Dwellings)

UENUM()
enum class EMEGRoad : uint8
{
	Up = 0,
	Right = 1,
	Down = 2,
	Left = 3
};
/**
 * 
 */
USTRUCT()
struct MEGALO_CPP_API FMEG_CellData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	EMEGDistrict DistrictType;

	UPROPERTY(EditDefaultsOnly)
	TArray<EMEGRoad> Roads;
};