// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/MEG_CellData.h"
#include "MEG_GridCell.generated.h"

class UWidgetComponent;
class UMEG_CellWidget;
struct FMEG_CardData;

UCLASS()
class MEGALO_CPP_API AMEG_GridCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMEG_GridCell();
	void UpdateCellWidget(EMEGDistrict DistrictType, TArray<EMEGRoad> _Roads);
	EMEGDistrict GetDistrictType() const;
	TArray<EMEGRoad> GetRoads() const;
	void SetCellVisibilityAndOpacity(bool bVisibility, float _Opacity);
	const UMEG_CellWidget* GetCellWidget() const;

	FVector2D CellCoords;
	TArray<EMEGRoad> Roads;


protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* CellWidget;
public:	

};
