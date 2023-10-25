// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "Engine/DataAsset.h"
#include "SMonsterData.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API USMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

/********************************* PROPERTIES ********************************/
public:
	/** Monster info is referencing this data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TSubclassOf<AActor> MonsterClass;

	/** Actions/Buffs to grant this monster */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TArray<TSubclassOf<USAction>> Actions;

	/** UI icon to represent the monster */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	UTexture2D* Icon;
	
	
/*********************************** METHODS *********************************/
public:
	/** ENGINE: Returns an Type:Name pair representing the PrimaryAssetId for this object */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
