// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "AI/SAICharacter.h"


// CVars
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);



// Sets default values
ASGameModeBase::ASGameModeBase(): SpawnBotQuery(nullptr), MaxNumOfBotsCurve(nullptr)
{
	// Set some values
	SpawnTimerInterval = 4.0f;

	// Changing PlayerState class to our custom one
	PlayerStateClass = ASPlayerState::StaticClass();
}


// GameMode class doesn't have BeginPlay() bc GM class is the responsible for calling BeginPlay on all the actors in our world
// But this is like the BeginPlay of GameMode class
void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Set up timers
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimeElapsed, SpawnTimerInterval, true);
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
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator());
		DrawDebugCapsule(GetWorld(),Locations[0],50,20,FQuat().Identity,FColor::Blue, false, 60);
	}
}


//
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
