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
	
	UPROPERTY(meta = (BindWidget))
	UMEG_CardWidget* FirstCard;

	UPROPERTY(meta = (BindWidget))
	UMEG_CardWidget* SecondCard;

	UPROPERTY(meta = (BindWidget))
	UMEG_CardWidget* ThirdCard;
};
