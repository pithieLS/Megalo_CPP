// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MEG_CellData.h"
#include "MEG_CellWidget.generated.h"

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_CellWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	EMEGDistrict DistrictType;

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* DistrictImage;
};
