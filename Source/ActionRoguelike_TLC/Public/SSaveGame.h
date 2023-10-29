// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"


USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	/** Identifier for which Actor this belongs to */
	UPROPERTY()
	FName ActorName;

	/** For movable Actors, keep location, rotation and scale */
	UPROPERTY()
	FTransform Transform;

	/** The whole actor info in binary */
	UPROPERTY()
	TArray<uint8> ByteData;
};


UCLASS()
class ACTIONROGUELIKE_TLC_API USSaveGame : public USaveGame
{
	GENERATED_BODY()

/********************************* PROPERTIES ********************************/
public:
	/** All the actors saved for the game */
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

	/** Higher round reach by the player */
	UPROPERTY()
	int RoundPersonalBest;

	/** Credits amount */
	UPROPERTY()
	int Credits;
	
	
/*********************************** METHODS *********************************/
public:
	USSaveGame();
};
