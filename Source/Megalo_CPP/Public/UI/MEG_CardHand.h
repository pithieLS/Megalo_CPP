// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MEG_CardHand.generated.h"

class UMEG_CardWidget;
/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_CardHand : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void UpdateHand();
	void FillCardsInHandArray();
	UFUNCTION()
	void OnCardSelected(int32 CardID);
	UFUNCTION()
	void OnRequestPlaceCard(FVector2D InCoords);


	UMEG_CardWidget* GetSelectedCard();

	TArray<UMEG_CardWidget*> CardsInHand;

	UPROPERTY(meta = (BindWidget))
	UMEG_CardWidget* FirstCard;

	UPROPERTY(meta = (BindWidget))
	UMEG_CardWidget* SecondCard;

	UPROPERTY(meta = (BindWidget))
	UMEG_CardWidget* ThirdCard;
};
