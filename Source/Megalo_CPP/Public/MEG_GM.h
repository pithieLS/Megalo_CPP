// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/MEG_CardData.h"
#include "MEG_GM.generated.h"

struct FMEG_DistrictDataRow;
/**
 * 
 */
UCLASS()
class MEGALO_CPP_API AMEG_GM : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	const FMEG_CardData* GetCardData(int32 _CardID) const;
	const FMEG_DistrictDataRow* GetDistrictDataRow(EMEGDistrict DistrictType) const;

//Card containers
	TArray<int32> DrawnCardID;
	TArray<int32> PlayedCardID;
	TArray<int32> ScoringCardID;

	UPROPERTY(EditDefaultsOnly)
	TArray<FMEG_CardData> Cards;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDWidgetClass;

	DECLARE_DELEGATE(FOnCardHandUpdated)
	FOnCardHandUpdated OnCardHandUpdatedDelegate;

	DECLARE_DELEGATE_OneParam(FOnCardSelected, int32 CardID)
	FOnCardSelected OnCardSelectedDelegate;

protected:
	void DrawCard();
	/* Return a card id from an available card. Return INDEX_NONE if no card is available */
	int32 GetAvailableCardID() const;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* DistrictDataTable;
};
