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
}

void UMEG_CardHand::NativeDestruct()
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	GameMode->OnCardHandUpdatedDelegate.Unbind();
}

void UMEG_CardHand::UpdateHand()
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode))
		return;

	const int32 NumCardsInHand = GameMode->DrawnCardID.Num();
	for (int32 Index = 0; Index < CardsInHand.Num(); Index++)
	{
		if (Index >= NumCardsInHand)
		{
			CardsInHand[Index]->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}
		CardsInHand[Index]->SetVisibility(ESlateVisibility::Visible);
		CardsInHand[Index]->UpdateCard(GameMode->DrawnCardID[Index]);
	}
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
