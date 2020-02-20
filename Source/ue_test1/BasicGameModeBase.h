// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "Enemy.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
//#include "Blueprint/UserWidget.h"
#include "BasicGameStateBase.h"
#include "OptionPtr.h"
#include "DifficultyLevel.h"

#include "BasicGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_TEST1_API ABasicGameModeBase : public AGameMode
{
	GENERATED_BODY()
//UE variables
public:
	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	TSubclassOf<class AEnemy> WeakEnemy;

	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	TSubclassOf<class AEnemy> MediumEnemy;

	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	TSubclassOf<class AEnemy> StrongEnemy;

	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	TSubclassOf<class AFlyingSpot> FugitiveFlyingSpot;

	UPROPERTY(EditDefaultsOnly, Category = TurretDestroyer)
	TSubclassOf<class ATurretDestroyer> TurretDestroyer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	int32 StartMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	int32 MoneyIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	int32 StartHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	int32 LosePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	int32 FugitivePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEnemy)
	float SpawnEnemyX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEnemy)
	float SpawnEnemyYBegin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEnemy)
	float SpawnEnemyYEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEnemy)
	float SpawnEnemyZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TurretDestroyer)
	float SpawnTurretDestroyerZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	FString turret1_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	FString turret2_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	FString turret3_name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	int32 turret1_price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	int32 turret2_price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	int32 turret3_price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	TArray<TSubclassOf<class UDifficultyLevel>> Levels;

//UE events and methods
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void HandleStartingNewPlayer(APlayerController* NewPlayer);
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
	virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform);

	virtual void Tick(float DeltaSeconds) override;

	virtual void Logout(AController* Exiting) override;

	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual void HandleLeavingMap() override;
	virtual void HandleMatchAborted() override;
	virtual void HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver) override;
public:
	ABasicGameModeBase();
//My variables
private:
	int32 current_level;
	float spawn_weak_enemy_freq;
	float spawn_medium_enemy_freq;
	float spawn_strong_enemy_freq;
	float spawn_turret_destroyer_freq;
	float level_time;
	float pre_spawn_counter;
	float spawn_delay;
	bool losed;

	OptionPtr<AEnemy> fugitive;
	OptionPtr<class AFlyingSpot> fugitive_flying_spot;
//My methods
private:
	OptionPtr<UWorld> get_world();
	OptionPtr<ABasicGameStateBase> get_game_state();

	void try_spawn_enemy(UWorld& world, TSubclassOf<class AEnemy>& enemy, float freq);
	void set_fugitive(UWorld& world, AEnemy& found_fugitive);
	void try_spawn_turret_destroyer(UWorld& world);
public:
	UFUNCTION(BlueprintCallable, Category = "Money")
	void restart_game();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	FString get_turret_name(uint8 turret_type);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	int32 get_turret_price(uint8 turret_type);

	UFUNCTION(BlueprintCallable)
	float get_enemy_dest_position();

	void set_difficulty_level(int32 level);

	FString create_difficulty_level_name();
//My Events
private:
	//Вызывается до старта матча и устанавливает акторы, значения переменным
	void on_before_match();

	//Вызывается при старте матча и запускает мир, эта функция более динамичная и работает быстро, не предполагает долгих настроек и синхронизации
	void on_start_match();

	//Вызывается при очищении матча -- убирает мусор вроде трупов и разрушений, значения сбрасываются к дефолтному значению
	void on_clear_match();

	//Вызывается каждый тик
	void on_tick(float dt);

	//Вызывается, если мы проиграли(все игроки мертвы)
	void on_losed();

	void on_spawn_enemies(UWorld& world);

	bool check_lose();

	void set_losed();

	void find_fugitive();

public:
	void reset_fugitive(AEnemy&);
};
