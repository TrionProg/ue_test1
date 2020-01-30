// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Enemy.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

#include "BasicGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_TEST1_API ABasicGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	TSubclassOf<class AEnemy> WeakEnemy;

	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	TSubclassOf<class AEnemy> MediumEnemy;

	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	TSubclassOf<class AEnemy> StrongEnemy;

public:
	ABasicGameModeBase();
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void Tick(float DeltaSeconds);
};
