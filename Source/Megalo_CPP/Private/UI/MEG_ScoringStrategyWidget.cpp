// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEG_ScoringStrategyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MEG_GM.h"
#include "Score/MEG_ScoringStrategy.h"
#include "Components/Image.h"
#include "Data/MEG_CardData.h"

void UMEG_ScoringStrategyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	const AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	ScoringImages.Add(ScoringImageOne);
	ScoringImages.Add(ScoringImageTwo);
	ScoringImages.Add(ScoringImageThree);

	for (int32 index = 0; index < GameMode->ScoringCardsID.Num(); index++)
	{
		const FMEG_CardData* _CardData = GameMode->GetCardData(GameMode->ScoringCardsID[index]);
		if (!ensure(_CardData != nullptr))
			return;

		const UMEG_ScoringStrategy* ScoringStrategyCDO = Cast<UMEG_ScoringStrategy>(_CardData->ScoringClass->GetDefaultObject());
		if (!ensure(ScoringStrategyCDO != nullptr))
			return;

		UTexture2D* _CardTexture = ScoringStrategyCDO->ScoringImage;
		if (!ensure(_CardTexture != nullptr))
			return;

		ScoringImages[index]->SetBrushFromTexture(_CardTexture);
	}
}