// Fill out your copyright notice in the Description page of Project Settings.


#include "SMonsterData.h"


// ENGINE: Returns an Type:Name pair representing the PrimaryAssetId for this object
FPrimaryAssetId USMonsterData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Monsters", GetFName());
}
