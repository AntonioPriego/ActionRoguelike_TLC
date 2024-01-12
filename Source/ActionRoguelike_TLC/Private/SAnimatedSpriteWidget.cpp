// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimatedSpriteWidget.h"

#include "Components/Image.h"


// 
void USAnimatedSpriteWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FramesPerSecond = 40.f;
	CurrentFrame    = 0;
	NumOfFrames     = 32; // Set the actual NumOfFrames later on this method
	bIsAnimForward  = true;
	bAnimCompleted  = false;
}


// 
void USAnimatedSpriteWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
		
	if (ImageToAnim)
	{
		ImageToAnim->SetBrushFromMaterial(SpriteSheet);
	}

	if (SpriteSheet)
	{
		float GridShapeX, GridShapeY;
		SpriteSheet->GetScalarParameterValue(FName("GridShapeX"), GridShapeX);
		SpriteSheet->GetScalarParameterValue(FName("GridShapeY"), GridShapeY);

		NumOfFrames = GridShapeX * GridShapeY;
	}
}


//
void USAnimatedSpriteWidget::AnimationTick()
{
	ImageToAnim->GetDynamicMaterial()->SetScalarParameterValue("CurrentFrame", CurrentFrame);
	
	if (CurrentFrame == 0 && CurrentFrame+(bIsAnimForward ? 1 : -1) < 0)
	{
		StopAnim();
	}
	else if (CurrentFrame == NumOfFrames-1 && CurrentFrame+(bIsAnimForward ? 1 : -1) > NumOfFrames-1)
	{
		bAnimCompleted = true;
		StopAnim();
	}
	else
	{
		CurrentFrame += (bIsAnimForward ? 1 : -1);
	}
}


//
void USAnimatedSpriteWidget::StartAnim()
{
	if (!bAnimCompleted)
	{
		bIsAnimForward = true;

		TurnOnAnimTick();
	}
}


//
void USAnimatedSpriteWidget::StopAnim()
{
	TurnOffAnimTick();
}


//
void USAnimatedSpriteWidget::ReverseAnim()
{
	bIsAnimForward = false;
	bAnimCompleted = false;

	if (!GetWorld()->GetTimerManager().IsTimerActive(Animation_TimerHandle))
	{		
		TurnOnAnimTick();
	}
}


//
void USAnimatedSpriteWidget::TurnOnAnimTick()
{
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "AnimationTick");	
	GetWorld()->GetTimerManager().SetTimer(Animation_TimerHandle, Delegate, 1.0f/FramesPerSecond, true);
}


//
void USAnimatedSpriteWidget::TurnOffAnimTick()
{
	GetWorld()->GetTimerManager().ClearTimer(Animation_TimerHandle);
}
