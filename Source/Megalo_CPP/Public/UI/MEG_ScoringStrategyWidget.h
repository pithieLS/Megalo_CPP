// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MEG_ScoringStrategyWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_ScoringStrategyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* ScoringImageOne;

	UPROPERTY(meta = (BindWidget))
	UImage* ScoringImageTwo;

	UPROPERTY(meta = (BindWidget))
	UImage* ScoringImageThree;
	
	TArray<UImage*> ScoringImages;
};
