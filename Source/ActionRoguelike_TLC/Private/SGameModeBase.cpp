// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "AI/SAICharacter.h"


// Sets default values
ASGameModeBase::ASGameModeBase()
{
	// Set some values
	SpawnTimerInterval = 2.0f;
}


// GameMode class doesn't have BeginPlay() bc GM class is the responsible for calling BeginPlay on all the actors in our world
// But this is like the BeginPlay of GameMode class
void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Set up timers
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimeElapsed, SpawnTimerInterval, true);
}


// Logic on TimerHandle_SpawnBots time elapsed, which means a Bot have to spawn
void ASGameModeBase::SpawnBotTimeElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery
	(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr
	);

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


	int32 NumOfAliveBots = 0;
	// This for is the equivalent to GetAllActorsOfClas on BP
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		const ASAICharacter* Bot = *It;

		USAttributesComponent* AttributeComponent = Cast<USAttributesComponent>(Bot->GetComponentByClass(USAttributesComponent::StaticClass()));		
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			NumOfAliveBots++;
		}

		if (NumOfAliveBots >= GetMaxNumOfBots(GetWorld()->TimeSeconds))
		{
			UE_LOG(LogTemp, Log, TEXT("Found >%i alive enemy bots.\nSkipping bot spawn."), NumOfAliveBots);
			return;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive enemy bots."), NumOfAliveBots);

	// Get locations from query
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	// Check a location is the expected
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator());
		DrawDebugCapsule(GetWorld(),Locations[0],50,20,FQuat().Identity,FColor::Blue, false, 60);
	}
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
