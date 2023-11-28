// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MEG_ScoringStrategy.generated.h"

class AMEG_GridCell;
class AMEG_GM;

/**
 * 
 */
UCLASS(Blueprintable)
class MEGALO_CPP_API UMEG_ScoringStrategy : public UObject
{
	GENERATED_BODY()
	
public:
	virtual int32 GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const { return 0; }

	UPROPERTY(EditDefaultsOnly)
	int32 ScoringGoal = -1;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* ScoringImage;
};

UCLASS(Blueprintable)
class MEGALO_CPP_API UMEG_ScoreGoGreen : public UMEG_ScoringStrategy
{
	GENERATED_BODY()

public:
	virtual int32 GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const override;

};

UCLASS(Blueprintable)
class MEGALO_CPP_API UMEG_ScoreBloomBloom : public UMEG_ScoringStrategy
{
	GENERATED_BODY()

public:
	virtual int32 GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const override;

};

UCLASS(Blueprintable)
class MEGALO_CPP_API UMEG_ScoreStackAndScrapers : public UMEG_ScoringStrategy
{
	GENERATED_BODY()

public:
	virtual int32 GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const override;

};

UCLASS(Blueprintable)
class MEGALO_CPP_API UMEG_ScoreMasterPlanned : public UMEG_ScoringStrategy
{
	GENERATED_BODY()

public:
	virtual int32 GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const override;

};

UCLASS(Blueprintable)
class MEGALO_CPP_API UMEG_BlockParty : public UMEG_ScoringStrategy
{
	GENERATED_BODY()

public:
	virtual int32 GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const override;

};

UCLASS(Blueprintable)
class MEGALO_CPP_API UMEG_CentralPerks : public UMEG_ScoringStrategy
{
	GENERATED_BODY()

public:
	virtual int32 GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const override;

};

UCLASS(Blueprintable)
class MEGALO_CPP_API UMEG_TheStrip : public UMEG_ScoringStrategy
{
	GENERATED_BODY()

public:
	virtual int32 GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const override;

};

//UCLASS(Blueprintable)
//class MEGALO_CPP_API UMEG_TheOutskirsts : public UMEG_ScoringStrategy
//{
//	GENERATED_BODY()
//
//public:
//	virtual int32 GetScore(const TArray<AMEG_GridCell*>& GridCells, AMEG_GM* GameMode) const override;
//
//};
