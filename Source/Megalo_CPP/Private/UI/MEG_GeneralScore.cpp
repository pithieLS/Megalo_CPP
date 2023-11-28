// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEG_GeneralScore.h"
#include "Components/TextBlock.h"
#include "MEG_GM.h"
#include "Kismet/GameplayStatics.h"

void UMEG_GeneralScore::NativeConstruct()
{
	Super::NativeConstruct();

	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	GameMode->OnScoreUpdatedDelegate.AddUObject(this, &UMEG_GeneralScore::UpdateGeneralScores);
}

void UMEG_GeneralScore::NativeDestruct()
{
	Super::NativeDestruct();

	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	GameMode->OnScoreUpdatedDelegate.RemoveAll(this);
}

void UMEG_GeneralScore::UpdateGeneralScores()
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	RoadScore->SetText(FText::AsNumber(GameMode->RoadScore * -1));
	ClustersScore->SetText(FText::AsNumber(GameMode->ClustersScore));
}
