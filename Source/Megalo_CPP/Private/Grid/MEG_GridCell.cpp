// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/MEG_GridCell.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "Data/MEG_CardData.h"
#include "UI/MEG_CellWidget.h"
#include "UI/MEG_CellWidget.h"
#include "Data/MEG_CellData.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AMEG_GridCell::AMEG_GridCell()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	CellWidget = CreateDefaultSubobject<UWidgetComponent>("CellWidget");
	CellWidget->SetupAttachment(RootComponent);

	U_RoadZone = CreateDefaultSubobject<UBoxComponent>(TEXT("U_RoadZone"));
	U_RoadZone->SetupAttachment(RootComponent);
	D_RoadZone = CreateDefaultSubobject<UBoxComponent>(TEXT("D_RoadZone"));
	D_RoadZone->SetupAttachment(RootComponent);
	L_RoadZone = CreateDefaultSubobject<UBoxComponent>(TEXT("L_RoadZone"));
	L_RoadZone->SetupAttachment(RootComponent);
	R_RoadZone = CreateDefaultSubobject<UBoxComponent>(TEXT("R_RoadZone"));
	R_RoadZone->SetupAttachment(RootComponent);

	UL_SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("UL_SpawnZone"));
	UL_SpawnZone->SetupAttachment(RootComponent);
	UR_SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("UR_SpawnZone"));
	UR_SpawnZone->SetupAttachment(RootComponent);
	DR_SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DR_SpawnZone"));
	DR_SpawnZone->SetupAttachment(RootComponent);
	DL_SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DL_SpawnZone"));
	DL_SpawnZone->SetupAttachment(RootComponent);

	SpawnZones = { U_RoadZone, D_RoadZone,
					L_RoadZone, R_RoadZone,
					UL_SpawnZone, UR_SpawnZone,
					DL_SpawnZone, DR_SpawnZone };
}

void AMEG_GridCell::BeginPlay()
{
	Super::BeginPlay();
}

void AMEG_GridCell::UpdateCellWidget(EMEGDistrict DistrictType, TArray<EMEGRoad> _Roads)
{
	if (UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject()))
	{
		CastCellWidget->UpdateCell(DistrictType, _Roads);
		Roads = _Roads;
	}
	for (UStaticMeshComponent* _SpawnedMesh : SpawnedMeshes)
	{
		_SpawnedMesh->DestroyComponent();
		SpawnedMeshes.Remove(_SpawnedMesh);
	}
	SpawnMeshes();
}

EMEGDistrict AMEG_GridCell::GetDistrictType() const
{
	if (const UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject()))
	{
		return CastCellWidget->DistrictType;
	}
	check(false);
	return EMEGDistrict::Parc;
}

TArray<EMEGRoad> AMEG_GridCell::GetRoads() const
{
	if (const UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject()))
	{
		return CastCellWidget->Roads;
	}
	check(false);
	return TArray<EMEGRoad>();
}

void AMEG_GridCell::SetCellVisibilityAndOpacity(bool bVisibility, float _Opacity)
{
	const UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject());
	if (!ensure(CastCellWidget != nullptr))
		return;

	CellWidget->SetVisibility(bVisibility);
	CastCellWidget->GetDistrictImageComponent()->SetOpacity(_Opacity);

	for (UStaticMeshComponent* _SpawnedMesh : SpawnedMeshes)
		_SpawnedMesh->SetVisibility(bVisibility);
}

const UMEG_CellWidget* AMEG_GridCell::GetCellWidget() const
{
	const UMEG_CellWidget* CastCellWidget = Cast<UMEG_CellWidget>(CellWidget->GetUserWidgetObject());
	if (!ensure(CastCellWidget != nullptr))
		return nullptr;

	return CastCellWidget;
}

void AMEG_GridCell::SpawnMeshes()
{
	for (UStaticMeshComponent* _SpawnedMesh : SpawnedMeshes)
		_SpawnedMesh->DestroyComponent();
	SpawnedMeshes.Empty();

	const EMEGDistrict DistrictType = GetDistrictType();

	TArray<UStaticMesh*> DistrictMeshes;
	int32 SpawnNb;
	FVector2D RandScaleRange;

	// Get a ref to the correct SM array
	if (DistrictType == EMEGDistrict::Parc)
	{
		DistrictMeshes = ParcSM;
		SpawnNb = 15;
		RandScaleRange = FVector2D(0.02, 0.03);
	}
	else if (DistrictType == EMEGDistrict::Commercial)
	{
		DistrictMeshes = CommercialSM;
		SpawnNb = 6;
		RandScaleRange = FVector2D(0.005, 0.005);
	}
	else if (DistrictType == EMEGDistrict::Industry)
	{
		DistrictMeshes = IndustrySM;
		SpawnNb = 5;
		RandScaleRange = FVector2D(0.01, 0.015);
	}
	else if (DistrictType == EMEGDistrict::Dwellings)
	{
		DistrictMeshes = DwellingsSM;
		SpawnNb = 6;
		RandScaleRange = FVector2D(0.01, 0.015);
	}

	TArray<UBoxComponent*> ValidSpawnZones = SpawnZones;
	TArray<EMEGRoad> _Roads = GetRoads();
	for (EMEGRoad _Road : _Roads)
	{
		if (_Road == EMEGRoad::Up)
			ValidSpawnZones.Remove(U_RoadZone);
		else if(_Road == EMEGRoad::Down)
			ValidSpawnZones.Remove(D_RoadZone);
		else if(_Road == EMEGRoad::Left)
			ValidSpawnZones.Remove(L_RoadZone);
		else if(_Road == EMEGRoad::Right)
			ValidSpawnZones.Remove(R_RoadZone);
	}

	for (int32 Index = 0; Index < SpawnNb; Index++)
	{
		UStaticMeshComponent* NewMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), NAME_None);

		const int32 RandIndexZone = FMath::RandRange(0, ValidSpawnZones.Num() - 1);
		UBoxComponent* RandSpawnZone = ValidSpawnZones[RandIndexZone];
		const FVector RandSpawnLocation = FMath::RandPointInBox(RandSpawnZone->GetLocalBounds().GetBox());
		const float RandScale = FMath::RandRange(RandScaleRange.X, RandScaleRange.Y);
		const int32 RandIndexRot = FMath::RandRange(0, SpawnRoatations.Num() - 1);
		const float RandRotation = SpawnRoatations[RandIndexRot];
		
		NewMeshComponent->AttachToComponent(RandSpawnZone, FAttachmentTransformRules::KeepRelativeTransform);
		NewMeshComponent->SetRelativeLocation(RandSpawnLocation);
		NewMeshComponent->SetRelativeScale3D(FVector(RandScale, RandScale, RandScale));
		NewMeshComponent->SetStaticMesh(DistrictMeshes[FMath::RandRange(0, DistrictMeshes.Num() - 1)]);
		NewMeshComponent->SetRelativeRotation(FRotator(0, RandRotation, 0));
		NewMeshComponent->RegisterComponent();
		SpawnedMeshes.Add(NewMeshComponent);
	}
}