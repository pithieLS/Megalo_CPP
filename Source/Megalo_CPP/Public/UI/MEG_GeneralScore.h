// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MEG_GeneralScore.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_GeneralScore : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void UpdateGeneralScores();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoadScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ClustersScore;
};
