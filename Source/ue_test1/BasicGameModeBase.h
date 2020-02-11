// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Enemy.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	int32 start_money;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	int32 money_increase;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void RestartGame();

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;
public:

public:
	ABasicGameModeBase();
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void Tick(float DeltaSeconds);

	int32 get_start_money();
	float get_money_increase();
	void restart_game();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void HandleStartingNewPlayer(APlayerController* NewPlayer);
	virtual void RestartPlayer(AController* NewPlayer);
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot);
	virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform);
	virtual void SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform);
	virtual void Logout(AController* Exiting);
	//SpawnDefaultPawnAtTransform

};
