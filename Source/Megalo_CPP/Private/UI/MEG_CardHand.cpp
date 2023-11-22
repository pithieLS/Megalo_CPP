// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEG_CardHand.h"
#include "MEG_GM.h"
#include "UI/MEG_CardWidget.h"
#include "Kismet/GameplayStatics.h"

void UMEG_CardHand::NativeConstruct()
{
	FillCardsInHandArray();

	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	GameMode->OnCardHandUpdatedDelegate.BindUObject(this, &UMEG_CardHand::UpdateHand);
	GameMode->OnCardSelectedDelegate.BindUObject(this, &UMEG_CardHand::OnCardSelected);
	GameMode->OnRequestPlaceCard.AddUObject(this, &UMEG_CardHand::OnRequestPlaceCard);
}

void UMEG_CardHand::NativeDestruct()
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	GameMode->OnCardHandUpdatedDelegate.Unbind();
	GameMode->OnCardSelectedDelegate.Unbind();
	GameMode->OnRequestPlaceCard.RemoveAll(this);
}

void UMEG_CardHand::UpdateHand()
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	const int32 NumCardsInHand = GameMode->DrawnCardsID.Num();
	for (int32 Index = 0; Index < CardsInHand.Num(); Index++)
	{
		if (Index >= NumCardsInHand)
		{
			CardsInHand[Index]->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}
		CardsInHand[Index]->SetVisibility(ESlateVisibility::Visible);
		CardsInHand[Index]->UpdateCard(GameMode->DrawnCardsID[Index]);
	}
}

void UMEG_CardHand::OnRequestPlaceCard(FVector2D _Coords)
{
	// Don't do anything if no card is selected
	const UMEG_CardWidget* SelectedCard = GetSelectedCard();
	if (SelectedCard == nullptr)
		return;

	if (SelectedCard->GetCardID() == INDEX_NONE)
		return;

	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	GameMode->PlaceCardFromHand(SelectedCard->GetCardID(), _Coords);
	OnCardSelected(-1);
}

void UMEG_CardHand::FillCardsInHandArray()
{
	CardsInHand.Add(FirstCard);
	CardsInHand.Add(SecondCard);
	CardsInHand.Add(ThirdCard);
}

void UMEG_CardHand::OnCardSelected(int32 CardID)
{
	for (UMEG_CardWidget* _CardWidget : CardsInHand)
	{
		if (_CardWidget->GetCardID() == CardID)
			_CardWidget->SetSelected(!_CardWidget->GetIsSelected());
		else
			_CardWidget->SetSelected(false);
	}
}

UMEG_CardWidget* UMEG_CardHand::GetSelectedCard()
{
	for (UMEG_CardWidget* _cardWidget : CardsInHand)
	{
		if (_cardWidget->GetIsSelected())
			return _cardWidget;
	}
	return nullptr;
}
