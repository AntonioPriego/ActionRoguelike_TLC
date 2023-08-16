// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


// Called when the game starts or when spawned
void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	// Run our BehaviorTree
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullprt! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);	
	}
}
