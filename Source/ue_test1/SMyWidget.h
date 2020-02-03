// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class UE_TEST1_API SMyWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMyWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMoney(int32 money);
};
