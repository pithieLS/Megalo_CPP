// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MEG_ScoringCard.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UMEG_ScoringCard::UpdateCardScore(int32 Score)
{
	CardScore->SetText(FText::AsNumber(Score));
}

void UMEG_ScoringCard::UpdateCardImage(UTexture2D* Texture)
{
	ScoringImage->SetBrushFromTexture(Texture);
}
