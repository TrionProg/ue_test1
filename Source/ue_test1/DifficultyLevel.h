// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DifficultyLevel.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UE_TEST1_API UDifficultyLevel : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LevelTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float WeakEnemySpawnFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float MediumEnemySpawnFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float StrongEnemySpawnFrequency;
public:
	UDifficultyLevel();
	~UDifficultyLevel();
};
