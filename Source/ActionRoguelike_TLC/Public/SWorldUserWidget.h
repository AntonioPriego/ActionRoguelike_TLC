// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "SWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
public:
	/** The actor is going to project this widget */
	UPROPERTY(BlueprintReadOnly, Category=UI, meta=(ExposeOnSpawn=true))
	AActor* AttachedActor;

	/** Vertical offset between AttachedActor and the actual widget (this) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UI)
	FVector WorldOffset;

protected:
	UPROPERTY(meta=(BindWidget))
	USizeBox* ParentSizeBox;
	
/*********************************** METHODS *********************************/
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
