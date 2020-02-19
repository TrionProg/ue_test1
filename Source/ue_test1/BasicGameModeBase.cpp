// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicGameModeBase.h"
#include "Spectator.h"
#include "MyPlayerController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BasicGameStateBase.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "MyPlayerController.h"
#include "Enemy.h"
#include "Math/UnrealMathUtility.h"

//UE events and methods

ABasicGameModeBase::ABasicGameModeBase(): Super() {
	PrimaryActorTick.bCanEverTick = true;

	//Use C++ Spectator
	DefaultPawnClass = ASpectator::StaticClass(); 
	PlayerControllerClass = AMyPlayerController::StaticClass();
}


void ABasicGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	UE_LOG(LogTemp, Warning, TEXT("InitGame %s %s"), *MapName, *Options);
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABasicGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) {
	UE_LOG(LogTemp, Warning, TEXT("PreLogin"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}
void ABasicGameModeBase::PostLogin(APlayerController* NewPlayer) {
	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	Super::PostLogin(NewPlayer);
}

void ABasicGameModeBase::BeginPlay() {
	Super::BeginPlay();
}

void ABasicGameModeBase::HandleStartingNewPlayer(APlayerController* NewPlayer) {
	UE_LOG(LogTemp, Warning, TEXT("HandleStartingNewPlayer"));
	Super::HandleStartingNewPlayer(NewPlayer);
}
void ABasicGameModeBase::RestartPlayer(AController* NewPlayer) {
	UE_LOG(LogTemp, Warning, TEXT("RestartPlayer"));
	Super::RestartPlayer(NewPlayer);
}
void ABasicGameModeBase::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) {
	UE_LOG(LogTemp, Warning, TEXT("RestartPlayerAtPlayerStart"));
	Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}
void ABasicGameModeBase::RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) {
	UE_LOG(LogTemp, Warning, TEXT("RestartPlayerAtTransform"));
	Super::RestartPlayerAtTransform(NewPlayer, SpawnTransform);
}
void ABasicGameModeBase::SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) {
	UE_LOG(LogTemp, Warning, TEXT("SpawnDefaultPawnAtTransform"));
	Super::SpawnDefaultPawnAtTransform(NewPlayer, SpawnTransform);
}
void ABasicGameModeBase::Logout(AController* Exiting) {
	UE_LOG(LogTemp, Warning, TEXT("Logout"));
	Super::Logout(Exiting);
}

void ABasicGameModeBase::Tick(float dt) {
	Super::Tick(dt);
	on_tick(dt);
}

void ABasicGameModeBase::HandleDisconnect (UWorld* InWorld,UNetDriver* NetDriver) {
	UE_LOG(LogTemp, Warning, TEXT("HandleDisconnect"));
	Super::HandleDisconnect(InWorld, NetDriver);
}

void ABasicGameModeBase::HandleMatchHasStarted() {
	Super::HandleMatchHasStarted();
	on_start_match();
}

void ABasicGameModeBase::HandleMatchHasEnded() {
	UE_LOG(LogTemp, Warning, TEXT("HandleMatchHasEnded"));
	Super::HandleMatchHasEnded();

	on_clear_match();
	UE_LOG(LogTemp, Warning, TEXT("TODO:How long reset?"));
}

void ABasicGameModeBase::HandleLeavingMap() {
	UE_LOG(LogTemp, Warning, TEXT("HandleLeavingMap"));
	Super::HandleLeavingMap();
}

void ABasicGameModeBase::HandleMatchAborted() {
	UE_LOG(LogTemp, Warning, TEXT("HandleMatchAborted"));
	Super::HandleMatchAborted();
}

void ABasicGameModeBase::HandleMatchIsWaitingToStart() {
	UE_LOG(LogTemp, Warning, TEXT("HandleMatchIsWaitingToStart"));
	Super::HandleMatchIsWaitingToStart();

	on_before_match();

	StartMatch();
}

//My methods

OptionPtr<UWorld> ABasicGameModeBase::get_world() {
	return OptionPtr<UWorld>::new_unchecked(GetWorld());
}

OptionPtr<ABasicGameStateBase> ABasicGameModeBase::get_game_state() {
	if (auto world = get_world().match()) {
		return OptionPtr<ABasicGameStateBase>::new_unchecked(world->GetGameState<ABasicGameStateBase>());
	}
	else {
		return OptionPtr<ABasicGameStateBase>::new_none();
	}
}

void ABasicGameModeBase::restart_game() {
	UE_LOG(LogTemp, Log, TEXT("Restarting match..."));
	EndMatch();

	SetMatchState(MatchState::WaitingToStart);
	UE_LOG(LogTemp, Log, TEXT("Match has been restarted"));
}

//TODO unreachable, exception, enum
FString ABasicGameModeBase::get_turret_name(uint8 turret_type) {
	switch (turret_type) {
	case 1:
		return turret1_name;
	case 2:
		return turret2_name;
	case 3:
		return turret3_name;
	}

	return TEXT("No such turret");
}

//TODO unreachable, exception, enum
int32 ABasicGameModeBase::get_turret_price(uint8 turret_type) {
	switch (turret_type) {
	case 1:
		return turret1_price;
	case 2:
		return turret2_price;
	case 3:
		return turret3_price;
	}

	return 0;
}

void ABasicGameModeBase::set_difficulty_level(int32 next_level) {
	if (Levels.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("No Difficulty Levels"));
		return;
	}

	if (!Levels.IsValidIndex(next_level)) {
		next_level = Levels.Num() - 1;
	}

	current_level = next_level;

	TSubclassOf<class UDifficultyLevel>& lvl = Levels[current_level];
	auto level = lvl.GetDefaultObject();

	spawn_weak_enemy_freq = level->WeakEnemySpawnFrequency;
	spawn_medium_enemy_freq = level->MediumEnemySpawnFrequency;
	spawn_strong_enemy_freq = level->StrongEnemySpawnFrequency;
	level_time = level->LevelTime;;
	pre_spawn_counter = 0;
	spawn_delay = level->SpawnDelay;
	losed = false;

	if (auto world = get_world().match()) {
		auto level_name = create_difficulty_level_name();

		for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter) {
			auto player_controller = (AMyPlayerController*)iter->Get(); //TODO safe cast

			if (player_controller) {
				player_controller->set_difficulty_level(level_name);
				player_controller->set_difficulty_level_time(level_time);
			}
		}
	}
}

FString ABasicGameModeBase::create_difficulty_level_name() {
	TSubclassOf<class UDifficultyLevel>& lvl = Levels[current_level];
	auto level = lvl.GetDefaultObject();

	//UE_LOG(LogTemp, Warning, TEXT("Level: %d %s"), current_level, level->Name);

	FString lvlname = level->Name;
	UE_LOG(LogTemp, Warning, TEXT("Level: %d %s"), current_level, *lvlname);

	auto level_name = FString::Printf(TEXT("#%d %s"), current_level, *lvlname);

	return level_name;
}

//My Events

void ABasicGameModeBase::on_before_match() {
	UE_LOG(LogTemp, Warning, TEXT("on_before_match"));

	if (auto world = get_world().match()) {
		for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter) {
			auto player_controller = (AMyPlayerController*)iter->Get(); //TODO safe cast
			
			if (player_controller) {
				player_controller->on_before_match();
				if (!player_controller) {
					UE_LOG(LogTemp, Warning, TEXT("AAA"));
					return;
				}
				player_controller->set_health(StartHealth);
				player_controller->set_money(StartMoney);
			}
		}

		/*
		for (float i = -400; i < 600; i += 130) {
			auto pos = FVector(800, i, 0);

			AActor* my_actor = (AActor*)world->SpawnActor(WeakEnemy, &pos);
		}
		*/
	}

	set_difficulty_level(0);

	current_level = 0;
}

void ABasicGameModeBase::on_start_match() {
	UE_LOG(LogTemp, Warning, TEXT("Start match"));
}

void ABasicGameModeBase::on_clear_match() {
	UE_LOG(LogTemp, Warning, TEXT("Clear match"));

	ResetLevel();

	UE_LOG(LogTemp, Warning, TEXT("End clear match"));
}

void ABasicGameModeBase::on_tick(float dt) {
	if (losed) {
		UE_LOG(LogTemp, Warning, TEXT("Losed =((("));
		on_losed();

		return;
	}

	if (check_lose()) {
		return;
	}

	auto money_increase = (float)MoneyIncrease * dt;

	if (auto world = get_world().match()) {
		for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter) {
			auto player_controller = (AMyPlayerController*)iter->Get(); //TODO safe cast

			if (player_controller) {
				player_controller->give_money(money_increase);
			}
		}
	}

	level_time -= dt;

	if (level_time < 0.0) {
		current_level += 1;

		//Will be limited automatically
		set_difficulty_level(current_level);
	}

	pre_spawn_counter += dt;

	if (pre_spawn_counter >= spawn_delay) {
		pre_spawn_counter = 0.0;

		if (auto world = get_world().match()) {
			on_spawn_enemies(*world);
		}
	}
}

void ABasicGameModeBase::on_spawn_enemies(UWorld& world) {
	try_spawn_enemy(world, WeakEnemy, spawn_weak_enemy_freq);
	try_spawn_enemy(world, MediumEnemy, spawn_medium_enemy_freq);
	try_spawn_enemy(world, StrongEnemy, spawn_strong_enemy_freq);
}

void ABasicGameModeBase::try_spawn_enemy(UWorld& world, TSubclassOf<class AEnemy>& enemy, float freq) {
	if (FMath::RandRange(0.0f, 1.0f) < freq) {
		auto y_position = FMath::RandRange(SpawnEnemyYBegin, SpawnEnemyYEnd);

		auto pos = FVector(SpawnEnemyX, y_position, SpawnEnemyZ);

		AActor* my_actor = (AActor*)world.SpawnActor(enemy, &pos);//TODO attribs
	}
}

void ABasicGameModeBase::on_losed() {
	restart_game();
}

bool ABasicGameModeBase::check_lose() {
	if (auto world = get_world().match()) {
		TArray<AActor*> found_actors;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), found_actors);

		for (AActor* abstract_actor : found_actors) {
			AEnemy* enemy = Cast<AEnemy>(abstract_actor);

			if (enemy != nullptr) {
				if (enemy->is_alive()) {
					auto location = enemy->GetActorLocation();

					if (location.X < LosePosition) {
						enemy->kill();

						auto all_are_dead = true;

						for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter) {
							auto player_controller = (AMyPlayerController*)iter->Get(); //TODO safe cast

							if (player_controller) {
								if (!player_controller->decrement_health()) {//���� ����� ���
									all_are_dead = false;
								}
							}
						}

						if (all_are_dead) {
							set_losed();
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

void ABasicGameModeBase::set_losed() {
	losed = true;
}