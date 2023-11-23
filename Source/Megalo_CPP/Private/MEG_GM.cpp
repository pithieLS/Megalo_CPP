// Fill out your copyright notice in the Description page of Project Settings.


#include "MEG_GM.h"
#include "MEG_Pawn.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Data/MEG_DistrictDataRow.h"
#include "Grid/MEG_GridManager.h"
#include "Score/MEG_ScoringStrategy.h"
#include "Grid/MEG_GridCell.h"
#include "Grid/MEG_CardPlacer.h"


#define MAX_CARDS_CARDS_IN_HAND 3
#define NUM_SCORING_CARDS 3

void AMEG_GM::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	if (!ensure(HUDWidget != nullptr))
		return;

	HUDWidget->AddToViewport();

	SetScoringCards();

	for (int32 Index = 0; Index < MAX_CARDS_CARDS_IN_HAND; Index++)
	{
		DrawCard();
	}		

	GridManager = GetWorld()->SpawnActor<AMEG_GridManager>(GridManagerClassBP);
	if (!ensure(GridManager))
		return;

	int32 FirstPlacedCardID = GetAvailableCardID();
	GridManager->PlaceCard(FirstPlacedCardID, FVector2D(0, 0));
	PlayedCardsID.Add(FirstPlacedCardID);
	UpdateScore();

	UpdateBoardLimits();
}

void AMEG_GM::PlaceCardFromHand(int32 InCardId, FVector2D InCoords)
{
	if (InCardId == INDEX_NONE)
		return;
	GridManager->PlaceCard(InCardId, InCoords);
	PlayedCardsID.Add(InCardId);
	RemoveCardFromHand(InCardId);
	DrawCard();
	UpdateScore();
}

AMEG_GridManager* AMEG_GM::GetGridManager()
{
	return GridManager;
}

void AMEG_GM::RemoveCardFromHand(int32 CardId)
{
	DrawnCardsID.Remove(CardId);
	OnCardHandUpdatedDelegate.ExecuteIfBound();
}

void AMEG_GM::SetScoringCards()
{
	TArray<FMEG_CardData> _AvailableScoringCards = Cards.FilterByPredicate([this](const FMEG_CardData& InCardData)
		{
			return InCardData.ScoringClass != nullptr
					&& !DrawnCardsID.Contains(InCardData.CardID)
					&& !PlayedCardsID.Contains(InCardData.CardID)
					&& !ScoringCardsID.Contains(InCardData.CardID);
		});

	for (int32 index = 0; index < NUM_SCORING_CARDS; index++)
	{
		if (!ensure(_AvailableScoringCards.Num() != 0))
			break;

		int32 CardIndex = rand() % _AvailableScoringCards.Num();
		ScoringCardsID.Add(_AvailableScoringCards[CardIndex].CardID);
		_AvailableScoringCards.RemoveAt(CardIndex);
	}
}

void AMEG_GM::UpdateBoardLimits()
{

	TArray<AActor*> FoundPlacer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMEG_CardPlacer::StaticClass(), FoundPlacer);

	for (const AActor* _Placer : FoundPlacer)
	{
		const AMEG_CardPlacer* _CastedPlacer = Cast<AMEG_CardPlacer>(_Placer);
		FVector _PlacerCoords = _CastedPlacer->GetActorLocation();

		// Right = x+
		// Left = x-
		// Up = y-
		// Down = y+

		if (_PlacerCoords.X > BoardLimits.MaxR)
			BoardLimits.MaxR = _PlacerCoords.X;
		else if (_PlacerCoords.X < BoardLimits.MaxL)
			BoardLimits.MaxL = _PlacerCoords.X;
		else if (_PlacerCoords.Y < BoardLimits.MaxU)
			BoardLimits.MaxU = _PlacerCoords.Y;
		else if (_PlacerCoords.Y > BoardLimits.MaxD)
			BoardLimits.MaxD = _PlacerCoords.Y;
	}
}

void AMEG_GM::UpdateScore()
{
	Score = 0;
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));

	for (EMEGDistrict District : TEnumRange<EMEGDistrict>())
	{
		Score += GridManager->GetBiggestDistrictClusterSize(District);
	}
 	Score -= GridManager->GetRoadCount();

	for (int32 index = 0; index < ScoringCardsID.Num(); index++)
	{
		const FMEG_CardData* CardData = GetCardData(ScoringCardsID[index]);
		if (!ensure(CardData != nullptr && CardData->ScoringClass != nullptr))
			continue;

		const UMEG_ScoringStrategy* ScoringStrategyCDO = Cast<UMEG_ScoringStrategy>(CardData->ScoringClass->GetDefaultObject());
		if (!ensure(ScoringStrategyCDO != nullptr))
			continue;

		Score += ScoringStrategyCDO->GetScore(GridManager->GetGridCells(), GameMode);
	}
	UpdateBoardLimits();
}

int32 AMEG_GM::GetPointGoal() const
{
	int32 PointGoal = 0;
	for (const int32 _CardID : ScoringCardsID)
	{
		const FMEG_CardData* CardData = GetCardData(_CardID);
		if (!ensure(CardData != nullptr && CardData->ScoringClass != nullptr))
			continue;

		const UMEG_ScoringStrategy* ScoringStrategyCDO = Cast<UMEG_ScoringStrategy>(CardData->ScoringClass->GetDefaultObject());
		if(!ensure(ScoringStrategyCDO != nullptr))
			continue;
		
		PointGoal += ScoringStrategyCDO->ScoringGoal;
	}

	return PointGoal;
}

int32 AMEG_GM::GetAvailableCardID() const
{
	const TArray<FMEG_CardData> _AvailableCardID = Cards.FilterByPredicate([this](const FMEG_CardData& InCardData)
		{
			return !(DrawnCardsID.Contains(InCardData.CardID)
					|| PlayedCardsID.Contains(InCardData.CardID)
					|| ScoringCardsID.Contains(InCardData.CardID));
		});

	if(_AvailableCardID.Num() == 0)
		return INDEX_NONE;

	const int32 CardIndex = rand() % _AvailableCardID.Num();
	return _AvailableCardID[CardIndex].CardID;
}

const FMEG_CardData* AMEG_GM::GetCardData(int32 _CardID) const
{
	const FMEG_CardData* CardData = Cards.FindByPredicate([_CardID](const FMEG_CardData& InCardData)
		{
			return InCardData.CardID == _CardID;
		});

	return CardData;
}

const FMEG_DistrictDataRow* AMEG_GM::GetDistrictDataRow(EMEGDistrict _DistrictType) const
{
	if (!ensure(DistrictDataTable))
		return nullptr;

	TArray<FName> RowNames = DistrictDataTable->GetRowNames();

	for (const FName _RowName : RowNames)
	{
		FMEG_DistrictDataRow* DistrictDataRow = DistrictDataTable->FindRow<FMEG_DistrictDataRow>(_RowName, "");

		if (DistrictDataRow && DistrictDataRow->DistrictType == _DistrictType)
		{
			return DistrictDataRow;
		}
	}
	return nullptr;
}

void AMEG_GM::DrawCard()
{
	const int32 _DrawnCardID = GetAvailableCardID();

	if (_DrawnCardID != INDEX_NONE)
		DrawnCardsID.Add(_DrawnCardID);

	OnCardHandUpdatedDelegate.ExecuteIfBound();
}
