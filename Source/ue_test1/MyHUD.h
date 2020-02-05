// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UE_TEST1_API UMyHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetMoney(int32 money);

	UFUNCTION(BlueprintImplementableEvent)
	void SetCurrentTurret(const FString& type, int32 price);
	
};