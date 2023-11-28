// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEG_ScoringStrategyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MEG_GM.h"
#include "Score/MEG_ScoringStrategy.h"
#include "Components/Image.h"
#include "Data/MEG_CardData.h"
#include "Components/TextBlock.h"
#include "UI/MEG_ScoringCard.h"

void UMEG_ScoringStrategyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	GameMode->OnScoreUpdatedDelegate.AddUObject(this, &UMEG_ScoringStrategyWidget::UpdateCardScore);

	ScoringCards.Add(CardOne);
	ScoringCards.Add(CardTwo);
	ScoringCards.Add(CardThree);

	for (int32 index = 0; index < ScoringCards.Num(); index++)
	{
		const FMEG_CardData* _CardData = GameMode->GetCardData(GameMode->ScoringCardsID[index]);
		if (!ensure(_CardData != nullptr && _CardData->ScoringClass != nullptr))
			return;

		const UMEG_ScoringStrategy* ScoringStrategyCDO = Cast<UMEG_ScoringStrategy>(_CardData->ScoringClass->GetDefaultObject());
		if (!ensure(ScoringStrategyCDO != nullptr))
			continue;

		UTexture2D* _CardTexture = ScoringStrategyCDO->ScoringImage;
		if (!ensure(_CardTexture != nullptr))
			continue;

		ScoringCards[index]->UpdateCardImage(_CardTexture);
	}
}

void UMEG_ScoringStrategyWidget::NativeDestruct()
{
	Super::NativeDestruct();

	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	GameMode->OnScoreUpdatedDelegate.RemoveAll(this);
}

void UMEG_ScoringStrategyWidget::UpdateCardScore()
{
	const AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	ScoringCards[0]->UpdateCardScore(GameMode->FirstCardScore);
	ScoringCards[1]->UpdateCardScore(GameMode->SecondCardScore);
	ScoringCards[2]->UpdateCardScore(GameMode->ThirdCardScore);
}