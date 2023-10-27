// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SMonsterData.h"
#include "SSaveGame.h"
#include "ActionRoguelike_TLC/ActionRoguelike_TLC.h"
#include "AI/SAICharacter.h"
#include "Engine/AssetManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


// CVars
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);



// Sets default values
ASGameModeBase::ASGameModeBase(): SpawnBotQuery(nullptr), MaxNumOfBotsCurve(nullptr)
{
	// Set some values
	SpawnTimerInterval = 4.0f;
	bSpawnEnemiesEnabled = true;
	SlotName = "SaveGame-S0";

	// Changing PlayerState class to our custom one
	PlayerStateClass = ASPlayerState::StaticClass();
}


// ENGINE
void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot;
	}
		
	LoadSaveGame();
}


// Loads an user stats
void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (!CurrentSaveGame)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}
		
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));


		// Temporal solution to LoadActors @todo
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "LoadActors");
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, false);
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		
		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}


// GameMode class doesn't have BeginPlay() bc GM class is the responsible for calling BeginPlay on all the actors in our world
// But this is like the BeginPlay of GameMode class
void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Set up timers
	if (bSpawnEnemiesEnabled)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimeElapsed,SpawnTimerInterval, true);
	}
}


// Manages actor killed event
void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	// Respawn management
	const ASCharacter* PlayerKilled = Cast<ASCharacter>(VictimActor);
	if (PlayerKilled)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		TObjectPtr<AController> PlayerKilledController = PlayerKilled->GetController();

		if (ensure(PlayerKilledController))
		{			
			UE_LOG(LogTemp, Log, TEXT("PlayerKilled: %s"), *GetNameSafe(PlayerKilledController));
		}
		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", PlayerKilledController);
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, SpawnTimerInterval, false);
	}

	// Credits for kill enemies management
	ASAICharacter* EnemyKilled  = Cast<ASAICharacter>(VictimActor);
	ASCharacter* PlayerKiller = Cast<ASCharacter>(Killer);
	if (EnemyKilled && PlayerKiller)
	{
		ASPlayerState* KillerPlayerState = PlayerKiller->GetSPlayerState();
		KillerPlayerState->AddCredits(EnemyKilled->GetCreditsValue());
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}


// ENGINE: Signals that a player is ready to enter the game, which may start it up
void ASGameModeBase::HandleStartingNewPlayer_Implementation_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PlayerState)
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
}


// Logic on TimerHandle_SpawnBots time elapsed, which means a Bot have to spawn
void ASGameModeBase::SpawnBotTimeElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'"));
		return;
	}
	
	// Run queries is expensive, so check if it is necessary to query before doing it
	if (!CheckNumberAliveBotsUnderMax())
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery (this, SpawnBotQuery,
		                                                 this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}


// This method is called when SpawnBotQuery is finished
void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	// Error management
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("EEnvQueryStatus not successed"));
		return;
	}

	// Get locations from query
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	// Check a location is the expected
	if (Locations.IsValidIndex(0))
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);
		
			// Get random enemy
			const int32 RandomIndex = FMath::RandRange(0, Rows.Num()-1);
			const FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
	}
}


// Called when Monster is loaded from DataTable
void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId MonsterId, FVector Location)
{
	LogOnScreen(this, "Finished loading", FColor::Blue);
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UE_LOG(LogTemp, Log, TEXT("Loading monster..."));
		USMonsterData* MonsterData = Cast<USMonsterData>(Manager->GetPrimaryAssetObject(MonsterId));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, Location, FRotator());
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)), FColor::Blue);
	
				// Grant special actions, buffs, etc
				USActionComponent* ActionComponent = NewBot->FindComponentByClass<USActionComponent>();
				if (ActionComponent)
				{
					for (const TSubclassOf<USAction> ActionClass : MonsterData->Actions)
					{
						ActionComponent->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
	}
}


// Called when timer for respawn is elapsed
void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
			
		RestartPlayer(Controller);
	}
}


// Check the number of alive bots (enemies) is under the maximum (curve). Return true only if the number is below the max
bool ASGameModeBase::CheckNumberAliveBotsUnderMax() const
{	
	int32 NumOfAliveBots = 0;
	// This for is the equivalent to GetAllActorsOfClas on BP
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		const ASAICharacter* Bot = *It;

		const USAttributesComponent* AttributesComponent = USAttributesComponent::GetAttributes(Bot);		
		if (ensure(AttributesComponent) && AttributesComponent->IsAlive())
		{
			NumOfAliveBots++;
		}

		if (NumOfAliveBots >= GetMaxNumOfBots(GetWorld()->TimeSeconds))
		{
			UE_LOG(LogTemp, Log, TEXT("Found >%i alive enemy bots (Maximum).\nSkipping bot spawn."), NumOfAliveBots);
			return false;
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Found %i alive enemy bots."), NumOfAliveBots);


	return true;
}


// Returns the max number of alive bots (enemies) in the world based on the CurveFloat
float ASGameModeBase::GetMaxNumOfBots(const float Seconds) const
{
	if (MaxNumOfBotsCurve)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("GetMaxNumOfBots() accesed and returned %f"),
		                                 MaxNumOfBotsCurve->GetFloatValue(Seconds)));
		return MaxNumOfBotsCurve->GetFloatValue(Seconds);
	}


	return -1;
}


// DEBUG: To quick kill all on testing
void ASGameModeBase::KillAll()
{
		for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    	{
    		const ASAICharacter* Bot = *It;
    
    		USAttributesComponent* AttributesComponent = USAttributesComponent::GetAttributes(Bot);		
    		if (ensure(AttributesComponent) && AttributesComponent->IsAlive())
    		{
    			AttributesComponent->Kill(this);
    		}
    	}
}


// Saves the user current state
void ASGameModeBase::WriteSaveGame()
{
	for (int32 i=0; i<GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PlayerState = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
		}
	}

	
	CurrentSaveGame->SavedActors.Empty();

	// Iterate the entire world of actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		//Only interested in our 'gameplay actors'
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetFName();
		ActorData.Transform = Actor->GetTransform();

		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Turquoise, FString::Printf(TEXT("%s"), *ActorData.ActorName.ToString()));

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Convert Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}


// Method where all the important/interactive actors
void ASGameModeBase::LoadActors() const
{
	// Iterate the entire world of actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		//Only interested in our 'gameplay actors'
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
		{
			if (ActorData.ActorName == Actor->GetFName())
			{
				Actor->SetActorTransform(ActorData.Transform);
				
				// Load the array to fill with data from Actor
				FMemoryReader MemReader(ActorData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				
				Ar.ArIsSaveGame = true;
				// Convert binary array back into Actor's UPROPERTIES 
				Actor->Serialize(Ar);

				ISGameplayInterface::Execute_OnActorLoaded(Actor);
				
				break;
			}
		}
	}
}