// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MEG_GridCell.generated.h"

class UWidgetComponent;
struct FMEG_CardData;
enum class EMEGDistrict;

UCLASS()
class MEGALO_CPP_API AMEG_GridCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMEG_GridCell();
	void UpdateCellDistrict(EMEGDistrict DistrictType);
	EMEGDistrict GetDistrictType() const;

	FVector2D CellCoords;


protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* CellWidget;
public:	

};
