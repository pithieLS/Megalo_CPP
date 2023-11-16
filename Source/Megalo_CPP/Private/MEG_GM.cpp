// Fill out your copyright notice in the Description page of Project Settings.


#include "MEG_GM.h"
#include "MEG_Pawn.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Data/MEG_DistrictDataRow.h"
#include "Grid/MEG_GridManager.h"


#define MAX_CARDS_CARDS_IN_HAND 3

void AMEG_GM::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	if (!ensure(HUDWidget != nullptr))
		return;

	HUDWidget->AddToViewport();

	for (int32 Index = 0; Index < MAX_CARDS_CARDS_IN_HAND; Index++)
	{
		DrawCard();
	}		

	GridManager = GetWorld()->SpawnActor<AMEG_GridManager>(GridManagerClassBP);
	if (!ensure(GridManager))
		return;

	const int32 FirstPlacedCardID = GetAvailableCardID();
	GridManager->PlaceCard(FirstPlacedCardID, FVector2D(0, 0));
	PlayedCardsID.Add(FirstPlacedCardID);
	UpdateScore();
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

void AMEG_GM::RemoveCardFromHand(int32 CardId)
{
	DrawnCardsID.Remove(CardId);
	OnCardHandUpdatedDelegate.ExecuteIfBound();
}

void AMEG_GM::UpdateScore()
{
	Score = 0;
	for (EMEGDistrict District : TEnumRange<EMEGDistrict>())
	{
		Score += GridManager->GetBiggestDistrictClusterSize(District);
	}
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
