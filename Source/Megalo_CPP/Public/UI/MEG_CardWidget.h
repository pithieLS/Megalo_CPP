// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MEG_CardWidget.generated.h"

class UMEG_CellWidget;
enum class EMEGCellPosition;

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_CardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateCard(int32 CardID);
	virtual void NativeConstruct() override;

protected:
	void FillCellWidgetMap();

	TMap<EMEGCellPosition, UMEG_CellWidget*> CellWidgets;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellTL;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellTR;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellBR;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellBL;

};
