// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MEG_HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UMEG_CardHand* WBP_Hand;
};
