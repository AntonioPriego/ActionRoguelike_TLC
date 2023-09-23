// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"


// Defines behavior at action start
void USAction::StartAction_Implementation(AActor* Instigator)
{
}


// Defines behavior at action end
void USAction::StopAction_Implementation(AActor* Instigator)
{
}

UWorld* USAction::GetWorld() const
{
	const UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}
