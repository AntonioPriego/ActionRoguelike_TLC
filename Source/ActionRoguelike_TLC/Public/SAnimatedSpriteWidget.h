// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SAnimatedSpriteWidget.generated.h"

class UImage;


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_TLC_API USAnimatedSpriteWidget : public UUserWidget
{
	GENERATED_BODY()


/********************************* PROPERTIES ********************************/
protected:	
	/** */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ImageToAnim;

	/** */
	UPROPERTY(EditAnywhere, Category=Image)
	TObjectPtr<UMaterial> SpriteSheet;
	
	/** */
	UPROPERTY(EditDefaultsOnly, Category=Animation)
	float FramesPerSecond;
	
	/** */
	UPROPERTY(BlueprintReadWrite, Category=Animation)
	int32 CurrentFrame;

	/** */
	bool bIsAnimForward;

	/** */
	bool bAnimCompleted;

	/** */
	UPROPERTY(BlueprintReadWrite, Category=Animation)
	int32 NumOfFrames;

	FTimerHandle Animation_TimerHandle;
	
/*********************************** METHODS *********************************/
public:
	/** */
	virtual void NativeConstruct() override;
	
	/** */
	virtual void NativePreConstruct() override;
	
protected:
	/** */
	UFUNCTION(BlueprintCallable, Category=Animation)
	void AnimationTick();
	
	/** */
	UFUNCTION(BlueprintCallable, Category=Animation)
	void StartAnim();
	
	/** */
	UFUNCTION(BlueprintCallable, Category=Animation)
	void StopAnim();
	
	/** */
	UFUNCTION(BlueprintCallable, Category=Animation)
	void ReverseAnim();

	void TurnOnAnimTick();
	void TurnOffAnimTick();
};
