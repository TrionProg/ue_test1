// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerController.h"

#include "MyPlayerState.generated.h"

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
	uint8 current_turret_type;
	
	
};
