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
	UE_LOG(LogTemp, Warning, TEXT("Gameplay Tick"));
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

//My Events

void ABasicGameModeBase::on_before_match() {
	UE_LOG(LogTemp, Warning, TEXT("on_before_match"));

	if (auto world = get_world().match()) {
		for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter) {
			auto player_controller = (AMyPlayerController*)iter->Get(); //TODO safe cast
			
			if (player_controller) {
				UE_LOG(LogTemp, Warning, TEXT("SET MONEY!"));
				player_controller->set_money(StartMoney);
			}
		}

		for (float i = -400; i < 600; i += 130) {
			auto pos = FVector(800, i, 0);

			AActor* my_actor = (AActor*)world->SpawnActor(WeakEnemy, &pos);
		}
	}
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
	auto money_increase = (float)MoneyIncrease * dt;

	if (auto world = get_world().match()) {
		for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter) {
			auto player_controller = (AMyPlayerController*)iter->Get(); //TODO safe cast

			if (player_controller) {
				player_controller->give_money(money_increase);
			}
		}
	}
}