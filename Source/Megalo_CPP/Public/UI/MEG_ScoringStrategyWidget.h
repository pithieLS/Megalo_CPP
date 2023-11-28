// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MEG_ScoringStrategyWidget.generated.h"

class UImage;
class UTextBlock;
class UMEG_ScoringCard;

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_ScoringStrategyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void UpdateCardScore();


protected:

	UPROPERTY(meta = (BindWidget))
	UMEG_ScoringCard* CardOne;

	UPROPERTY(meta = (BindWidget))
	UMEG_ScoringCard* CardTwo;

	UPROPERTY(meta = (BindWidget))
	UMEG_ScoringCard* CardThree;

	TArray<UMEG_ScoringCard*> ScoringCards;
};
