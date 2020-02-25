// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerController.h"
//#include "Turret.h"

#include "MyPlayerState.generated.h"

enum class ETurretType : uint8;

/**
 * 
 */
UCLASS()
class UE_TEST1_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
friend class AMyPlayerController;

	UPROPERTY() //TODO Replication
	float money;

	UPROPERTY() //TODO Replication
	ETurretType current_turret_type;

	UPROPERTY() //TODO Replication
	int32 health;

	UPROPERTY() //TODO Replication
	float level_time;
	
	
};
