// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/MEG_CardData.h"
#include "MEG_GM.generated.h"

struct FMEG_DistrictDataRow;
class AMEG_GridManager;
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
	void PlaceCardFromHand(int32 InCardId, FVector2D InCoords);


//Card containers
	TArray<int32> DrawnCardsID;
	TArray<int32> PlayedCardsID;
	TArray<int32> ScoringCardsID;

	UPROPERTY(EditDefaultsOnly)
	TArray<FMEG_CardData> Cards;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDWidgetClass;

	DECLARE_DELEGATE(FOnCardHandUpdated)
	FOnCardHandUpdated OnCardHandUpdatedDelegate;

	DECLARE_DELEGATE_OneParam(FOnCardSelected, int32 CardID)
	FOnCardSelected OnCardSelectedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRequestPlaceCard, FVector2D Coords)
	FOnRequestPlaceCard OnRequestPlaceCard;

protected:
	void DrawCard();
	void UpdateScore();
	/* Return a card id from an available card. Return INDEX_NONE if no card is available */
	int32 GetAvailableCardID() const;
	void RemoveCardFromHand(int32 CardId);

	class AMEG_GridManager* GridManager;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* DistrictDataTable;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMEG_GridManager> GridManagerClassBP;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Score;
};
