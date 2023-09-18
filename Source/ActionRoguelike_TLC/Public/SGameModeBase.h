// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCreditsComponent.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:
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

	
	// Timers
	FTimerHandle TimerHandle_SpawnBots;

	
/*********************************** METHODS *********************************/
public:
	// Sets default values
	ASGameModeBase();
	
	// GameMode class doesn't have BeginPlay() bc GM class is the responsible for calling BeginPlay on all the actors in our world
	// But this is like the BeginPlay of GameMode class
	virtual void StartPlay() override;

	/** Manages actor killed event */
	UFUNCTION()
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	
protected:
	// Logic on TimerHandle_SpawnBots time elapsed, which means a Bot have to spawn
	UFUNCTION()
	void SpawnBotTimeElapsed();

	/** Called when SpawnBotQuery is finished */
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	/** */
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	/** Check the number of alive bots (enemies) is under the maximum (curve). Return true only if the number is below the max */
	bool CheckNumberAliveBotsUnderMax() const;

	/** Returns the max number of alive bots (enemies) in the world based on the CurveFloat */
	float GetMaxNumOfBots(const float Seconds) const;
	
	/** Get CreditsComponent from actor */
	USCreditsComponent* GetCreditsComponent(const APawn* Character) const;

	
/************************************ DEBUG **********************************/
public:
	/** DEBUG: To quick kill all on testing */
	UFUNCTION(Exec)
	void KillAll();
};

