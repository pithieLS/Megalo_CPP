// Fill out your copyright notice in the Description page of Project Settings.


#include "MEG_GM.h"
#include "MEG_Pawn.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/UserWidget.h"

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
}

int32 AMEG_GM::GetAvailableCardID() const
{
	const TArray<FMEG_CardData> _AvailableCardID = Cards.FilterByPredicate([this](const FMEG_CardData& InCardData)
		{
			return !(DrawnCardID.Contains(InCardData.CardID)
					|| PlayedCardID.Contains(InCardData.CardID)
					|| ScoringCardID.Contains(InCardData.CardID));
		});

	if(_AvailableCardID.Num() == 0)
		return INDEX_NONE;

	const int32 CardIndex = rand() % _AvailableCardID.Num();
	return _AvailableCardID[CardIndex].CardID;
}

void AMEG_GM::DrawCard()
{
	const int32 _DrawnCardID = GetAvailableCardID();

	if (_DrawnCardID != INDEX_NONE)
		DrawnCardID.Add(_DrawnCardID);

	OnCardHandUpdatedDelegate.ExecuteIfBound();
}
