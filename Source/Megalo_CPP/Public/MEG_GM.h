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
	AMEG_GridManager* GetGridManager();

	UFUNCTION(BlueprintCallable)
	int32 GetPointGoal() const;

	struct FHeightWidthLimits
	{
		float MaxU = 0;
		float MaxR = 0;
		float MaxD = 0;
		float MaxL = 0;
	} BoardLimits;

	int32 FirstCardScore = 0;
	int32 SecondCardScore = 0;
	int32 ThirdCardScore = 0;
	int32 RoadScore = 0;
	int32 ClustersScore = 0;

//Card containers
	TArray<int32> DrawnCardsID;
	TArray<int32> PlayedCardsID;
	TArray<int32> ScoringCardsID;
	class UMEG_HUDWidget* HUDWidget;

	UPROPERTY(EditDefaultsOnly)
	TArray<FMEG_CardData> Cards;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDWidgetClass;

	DECLARE_DELEGATE(FOnCardHandUpdated)
	FOnCardHandUpdated OnCardHandUpdatedDelegate;

	DECLARE_DELEGATE(FOnCardPlaced)
	FOnCardPlaced OnCardPlaced;

	DECLARE_DELEGATE(FOnRotatePressed)
	FOnRotatePressed OnRotatePressedDelegate;

	DECLARE_MULTICAST_DELEGATE(FOnScoreUpdated)
	FOnScoreUpdated OnScoreUpdatedDelegate;

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
	void SetScoringCards();
	void UpdateBoardLimits();

	class AMEG_GridManager* GridManager;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* DistrictDataTable;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMEG_GridManager> GridManagerClassBP;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Score;
};
