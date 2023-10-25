// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SGameModeBase.generated.h"

class USMonsterData;
class USSaveGame;


UCLASS()
class ACTIONROGUELIKE_TLC_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


	/********************************* PROPERTIES ********************************/
protected:
	/** All available monsters */
	UPROPERTY(EditDefaultsOnly, Category=AI)
	UDataTable* MonsterTable;
	
	/** Point to the current SSaveGame */
	UPROPERTY()
	USSaveGame* CurrentSaveGame;
	
	/** This property set the time between a spawn bot and the next one */
	UPROPERTY(EditDefaultsOnly, Category=AI)
	float SpawnTimerInterval;

	/** Custom query to spawn bot */
	UPROPERTY(EditDefaultsOnly, Category=AI)
	UEnvQuery* SpawnBotQuery;

	/** Set the class to spawn as minion */
	UPROPERTY(EditDefaultsOnly, Category=AI)
	TSubclassOf<AActor> MinionClass;

	/** Set the maximum number of alive bots (enemies) in the world */
	UPROPERTY(EditDefaultsOnly, Category=AI)
	UCurveFloat* MaxNumOfBotsCurve;

	/** Set the maximum number of alive bots (enemies) in the world */
	UPROPERTY(EditDefaultsOnly, Category=AI)
	bool bSpawnEnemiesEnabled;
	
	FString SlotName;
	
	// Timers
	FTimerHandle TimerHandle_SpawnBots;

	
	/*********************************** METHODS *********************************/
public:
	// Sets default values
	ASGameModeBase();

	/** ENGINE */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	// GameMode class doesn't have BeginPlay() bc GM class is the responsible for calling BeginPlay on all the actors in our world
	// But this is like the BeginPlay of GameMode class
	virtual void StartPlay() override;

	/** Manages actor killed event */
	UFUNCTION()
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	/** ENGINE: Signals that a player is ready to enter the game, which may start it up */
	UFUNCTION(BlueprintNativeEvent, Category=Game)
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	
protected:	
	/** Logic on TimerHandle_SpawnBots time elapsed, which means a Bot have to spawn */
	UFUNCTION()
	void SpawnBotTimeElapsed();

	/** Called when SpawnBotQuery is finished */
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	/** Called when timer for respawn is elapsed */
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	/** Called when Monster is loaded from DataTable */
	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId MonsterId, FVector Location);

	/** Check the number of alive bots (enemies) is under the maximum (curve). Return true only if the number is below the max */
	bool CheckNumberAliveBotsUnderMax() const;

	/** Returns the max number of alive bots (enemies) in the world based on the CurveFloat */
	float GetMaxNumOfBots(const float Seconds) const;

	/** Method where all the important/interactive actors
	 * @todo Not working properly with 'Spatially Loaded' (World partition feature) Actors which is default true on UE5; Because Spatially Loaded Actors are not available during GameModeBase::InitGame
	 * @tmp  I'm executing LoadActors with 0.2s delay to give GameModeBase::InitGame time to finish before trying to get Spatially Loaded Actors 
	 */
	UFUNCTION()
	void LoadActors() const;

	
	/************************************ DEBUG **********************************/
public:
	/** DEBUG: To quick kill all on testing */
	UFUNCTION(Exec)
	void KillAll();

	/** Saves the user current state */
	UFUNCTION(BlueprintCallable, Category=SaveGame)
	void WriteSaveGame();
	
	/** Loads an user state */
	UFUNCTION()
	void LoadSaveGame();
};

/** DataTable Row for spawning monsters in game mode */
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterInfoRow()
	: Weight(1.0f), SpawnCost(5.0f), KillReward(20.0f) {}
	
	/** Monster class and some of its properties */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	/** Relative chance to pick this monster */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	/** Points required by GameMode to spawn this unit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	/** Amount of credits awarded to killer of this unit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};