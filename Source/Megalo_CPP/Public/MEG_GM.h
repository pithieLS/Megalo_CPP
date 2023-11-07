// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/MEG_CardData.h"
#include "MEG_GM.generated.h"

/**
 * 
 */
UCLASS()
class MEGALO_CPP_API AMEG_GM : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FMEG_CardData> Cards;
};
