// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MEG_ScoringCard.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_ScoringCard : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateCardScore(int32 Score);
	void UpdateCardImage(UTexture2D* Texture);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* ScoringImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CardScore;
};
