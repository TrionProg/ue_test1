// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BasicGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_TEST1_API ABasicGameStateBase : public AGameState
{
	GENERATED_BODY()
protected:
	friend class ABasicGameModeBase;

	UPROPERTY() //TODO Replication
	float level_time;
public:
	float get_level_time();
};
