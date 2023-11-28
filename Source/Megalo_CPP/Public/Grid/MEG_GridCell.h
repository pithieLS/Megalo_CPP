// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/MEG_CellData.h"
#include "MEG_GridCell.generated.h"

class UWidgetComponent;
class UMEG_CellWidget;
class UStaticMesh;
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
	void SpawnMeshes();

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere)
	UBoxCollision* UL_RoadZone

	UPROPERTY(EditAnywhere)
	UWidgetComponent* CellWidget;

	UPROPERTY(EditAnywhere, Category = "DistrictMeshes")
	TArray<UStaticMesh*> ParcSM;

	UPROPERTY(EditAnywhere, Category = "DistrictMeshes")
	TArray<UStaticMesh*> CommercialSM;

	UPROPERTY(EditAnywhere, Category = "DistrictMeshes")
	TArray<UStaticMesh*> IndustrySM;

	UPROPERTY(EditAnywhere, Category = "DistrictMeshes")
	TArray<UStaticMesh*> DwellingsSM;

public:	

};
