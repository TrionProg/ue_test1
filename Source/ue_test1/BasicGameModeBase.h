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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	int32 StartMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayConfig)
	int32 MoneyIncrease;

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
//My methods
private:
	OptionPtr<UWorld> get_world();
	OptionPtr<ABasicGameStateBase> get_game_state();
public:
	UFUNCTION(BlueprintCallable, Category = "Money")
	void restart_game();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	FString get_turret_name(uint8 turret_type);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	int32 get_turret_price(uint8 turret_type);
//My Events
public:
	//Вызывается до старта матча и устанавливает акторы, значения переменным
	void on_before_match();

	//Вызывается при старте матча и запускает мир, эта функция более динамичная и работает быстро, не предполагает долгих настроек и синхронизации
	void on_start_match();

	//Вызывается при очищении матча -- убирает мусор вроде трупов и разрушений, значения сбрасываются к дефолтному значению
	void on_clear_match();

	//Вызывается каждый тик
	void on_tick(float dt);
};
