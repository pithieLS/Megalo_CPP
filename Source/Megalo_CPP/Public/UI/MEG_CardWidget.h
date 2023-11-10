// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MEG_CardWidget.generated.h"

class UMEG_CellWidget;
enum class EMEGCellPosition;
class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API UMEG_CardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateCard(int32 _CardID);
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	int32 GetCardID();
	bool GetIsSelected();
	void SetSelected(bool _IsSelected);

protected:
	void FillCellWidgetMap();

	UFUNCTION()
	void OnButtonClicked();

	int32 CardID;
	bool IsCardSelected = false;

	TMap<EMEGCellPosition, UMEG_CellWidget*> CellWidgets;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellTL;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellTR;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellBR;

	UPROPERTY(meta = (BindWidget))
	UMEG_CellWidget* CellBL;

	UPROPERTY(meta = (BindWidget))
	UButton* CardButton;

	UPROPERTY(meta = (BindWidget))
	UImage* SelectedImage;

};
