// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MEG_CellData.h"
#include "MEG_CellWidget.generated.h"

class UImage;
class USizeBox;
class UOverlay;
/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_CellWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateCell(EMEGDistrict _DistrictType, TArray<EMEGRoad> _Roads);
	UImage* GetDistrictImageComponent() const;

	UPROPERTY(EditDefaultsOnly)
	TArray<EMEGRoad> Roads;

	EMEGDistrict DistrictType;

protected:
	void UpdateDistrict(EMEGDistrict _DistrictType);
	void UpdateRoads(TArray<EMEGRoad> _Roads);

	UPROPERTY(meta = (BindWidget))
	UImage* DistrictImage;
	
	UPROPERTY(meta = (BindWidget))
	USizeBox* UpBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* RightBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* DownBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* LeftBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* MiddleBox;

};
