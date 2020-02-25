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
#include "TurretType.h"
#include "Turret.h"
#include "Math/UnrealMathUtility.h"
#include "FlyingSpot.h"
#include "TurretDestroyer.h"
#include "DifficultyLevel.h"
#include "BasicGameStateBase.h"

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
FString ABasicGameModeBase::get_turret_name(ETurretType turret_type) {
	switch (turret_type) {
	case ETurretType::Machinegun:
		return turret1_name;
	case ETurretType::Freeze:
		return turret2_name;
	case ETurretType::Pierce:
		return turret3_name;
	}

	return TEXT("No such turret"); //TODO unreachable
}

//TODO unreachable, exception, enum
int32 ABasicGameModeBase::get_turret_price(ETurretType turret_type) {
	switch (turret_type) {
	case ETurretType::Machinegun:
		return turret1_price;
	case ETurretType::Freeze:
		return turret2_price;
	case ETurretType::Pierce:
		return turret3_price;
	}

	return 0; //TODO unreachable
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
	spawn_turret_destroyer_freq = level->TurretDestroyerSpawnFrequency;
	pre_spawn_counter = 0;
	spawn_delay = level->SpawnDelay;
	losed = false;

	if (auto game_state = get_game_state().match()) {
		game_state->level_time = level->LevelTime;
	}

	if (auto world = get_world().match()) {
		auto level_name = create_difficulty_level_name();

		for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter) {
			auto player_controller = (AMyPlayerController*)iter->Get(); //TODO safe cast

			if (player_controller) {
				player_controller->set_difficulty_level(level_name);
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

	fugitive = OptionPtr<AEnemy>::new_none();
	fugitive_flying_spot = OptionPtr<AFlyingSpot>::new_none();
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

	if (fugitive.is_none()) {
		find_fugitive();
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

	if (auto game_state = get_game_state().match()) {
		game_state->level_time -= dt;

		if (game_state->level_time < 0.0) {
			current_level += 1;

			//Will be limited automatically
			set_difficulty_level(current_level);
		}
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
	try_spawn_turret_destroyer(world);
}

void ABasicGameModeBase::try_spawn_enemy(UWorld& world, TSubclassOf<class AEnemy>& enemy, float freq) {
	if (FMath::RandRange(0.0f, 1.0f) <= freq) {
		auto y_position = FMath::RandRange(SpawnEnemyYBegin, SpawnEnemyYEnd);

		auto pos = FVector(SpawnEnemyX, y_position, SpawnEnemyZ);

		AActor* my_actor = (AActor*)world.SpawnActor(enemy, &pos);//TODO attribs
	}
}

void ABasicGameModeBase::try_spawn_turret_destroyer(UWorld& world) {
	if (FMath::RandRange(0.0f, 1.0f) <= spawn_turret_destroyer_freq) {
		TArray<AActor*> found_actors;
		TArray<ATurret*> found_turrets;

		UGameplayStatics::GetAllActorsOfClass(&world, ATurret::StaticClass(), found_actors);

		for (AActor* abstract_actor : found_actors) {
			ATurret* turret = Cast<ATurret>(abstract_actor);

			if (turret != nullptr) {
				found_turrets.Add(turret);
			}
		}

		if (found_turrets.Num() > 0) {
			auto index = FMath::RandRange(0, found_turrets.Num());

			if (found_turrets.IsValidIndex(index)) {
				auto turret = found_turrets[index];

				auto turret_location = turret->GetActorLocation();
				auto pos = FVector(SpawnEnemyX, turret_location.Y, SpawnTurretDestroyerZ);

				AActor* my_actor = (AActor*)world.SpawnActor(TurretDestroyer, &pos);//TODO attribs
			}
		}
	}
}

void ABasicGameModeBase::on_losed() {
	restart_game();
}

bool ABasicGameModeBase::check_lose() {
	if (auto world = get_world().match()) {
		TArray<AActor*> found_actors;

		UGameplayStatics::GetAllActorsOfClass(world, AEnemy::StaticClass(), found_actors);

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
								if (!player_controller->decrement_health()) {//этот игрок жив
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

void ABasicGameModeBase::find_fugitive() {
	if (auto world = get_world().match()) {
		TArray<AActor*> found_actors;

		UGameplayStatics::GetAllActorsOfClass(world, AEnemy::StaticClass(), found_actors);

		auto found_fugitive = OptionPtr<AEnemy>::new_none();
		auto fugitive_dist = 0;

		for (AActor* abstract_actor : found_actors) {
			AEnemy* enemy = Cast<AEnemy>(abstract_actor);

			if (enemy != nullptr) {
				if (enemy->is_alive()) {
					auto location = enemy->GetActorLocation();

					if (location.X < FugitivePosition) {
						auto dist = FugitivePosition - location.X;

						if (dist > fugitive_dist) {
							fugitive_dist = dist;
							found_fugitive.set(enemy);
						}
					}
				}
			}
		}

		if (auto fnd_fugitive = found_fugitive.match()) {
			set_fugitive(*world, *fnd_fugitive);
		}
	}
}

void ABasicGameModeBase::set_fugitive(UWorld& world, AEnemy& found_fugitive) {
	fugitive.set(&found_fugitive);

	auto enemy_location = found_fugitive.GetActorLocation();
	auto fugitive_flying_spot_lication = FVector(
		enemy_location.X,
		enemy_location.Y,
		enemy_location.Z + 10000 //TODO HOVER HEIGHT
	);

	AActor* my_actor = (AActor*)world.SpawnActor(FugitiveFlyingSpot, &fugitive_flying_spot_lication);//TODO attribs

	if (auto flying_spot = (AFlyingSpot*)world.SpawnActor(FugitiveFlyingSpot, &fugitive_flying_spot_lication)) {
		flying_spot->set_hover_on_target(*(AActor*)&found_fugitive);
		fugitive_flying_spot.set(flying_spot);
	}

	//TODO may be calculate level of danger?

	TArray<AActor*> found_actors;

	UGameplayStatics::GetAllActorsOfClass(&world, ATurret::StaticClass(), found_actors);

	for (AActor* abstract_actor : found_actors) {
		ATurret* turret = Cast<ATurret>(abstract_actor);

		if (turret != nullptr) {
			turret->set_target(found_fugitive);
		}
	}
}

void ABasicGameModeBase::reset_fugitive(AEnemy& enemy) {
	if (auto current_fugitive = fugitive.match()) {
		if (current_fugitive == &enemy) {
			fugitive.reset();

			if (auto flying_spot = fugitive_flying_spot.match()) {
				flying_spot->Destroy();
				fugitive_flying_spot.reset();
			}
		}
	}
}

float ABasicGameModeBase::get_enemy_dest_position() {
	return LosePosition - 100.0;
}