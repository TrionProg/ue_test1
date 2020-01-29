// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BasicGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_TEST1_API ABasicGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABasicGameModeBase();
	
	virtual void Tick(float DeltaSeconds);
};
