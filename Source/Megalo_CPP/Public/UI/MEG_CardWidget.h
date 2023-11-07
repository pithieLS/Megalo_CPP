// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MEG_CardWidget.generated.h"

class UMEG_CellWidget;

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_CardWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellTL;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellTR;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellBR;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellBL;

};
