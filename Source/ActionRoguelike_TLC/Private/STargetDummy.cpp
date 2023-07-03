// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"


// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Basic set up for MeshComponent
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;

	// Basic set up for AttributesComponent
	AttributesComponent = CreateDefaultSubobject<USAttributesComponent>("AttributeComponent");
	

	//AttributesComponent->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}


void ASTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Trigger when health is changed (damage/healing)
	// RECOMMENDED bind Events on PostInitializeComponents
	AttributesComponent->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}


// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
		MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
}