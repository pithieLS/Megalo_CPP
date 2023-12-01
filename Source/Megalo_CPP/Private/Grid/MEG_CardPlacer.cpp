// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/MEG_CardPlacer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MEG_GM.h"

// Sets default values
AMEG_CardPlacer::AMEG_CardPlacer()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);
}

void AMEG_CardPlacer::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnClicked.AddDynamic(this, &AMEG_CardPlacer::OnMeshClicked);
}

void AMEG_CardPlacer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	MeshComponent->OnClicked.RemoveAll(this);
}

void AMEG_CardPlacer::OnMeshClicked(UPrimitiveComponent* Component, FKey ButtonPressed)
{
	AMEG_GM* GameMode = Cast<AMEG_GM>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr))
		return;

	GameMode->OnRequestPlaceCard.Broadcast(Coords);
}

void AMEG_CardPlacer::OnCardPlaced()
{
	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(PoofFX, RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
	NiagaraComp->Activate(true);
}
