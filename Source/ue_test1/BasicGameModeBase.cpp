// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicGameModeBase.h"
#include "Spectator.h"
#include "MyPlayerController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BasicGameStateBase.h"
#include "Runtime/Engine/Classes/Engine/World.h"


ABasicGameModeBase::ABasicGameModeBase(): Super() {
	PrimaryActorTick.bCanEverTick = true;

	//Use C++ Spectator
	DefaultPawnClass = ASpectator::StaticClass(); 
	PlayerControllerClass = AMyPlayerController::StaticClass();

	//should_restart = false;

	/*
	//Use blueprint Specrator
	static ConstructorHelpers::FClassFinder<APawn> FoundFPC(TEXT("/Game/Blueprints/SpectatorBlueprint"));
	if (FoundFPC.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found FPC"));

		DefaultPawnClass = (UClass*)FoundFPC.Class;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FPC Not found"));
		DefaultPawnClass = nullptr;
	}
	*/
}


void ABasicGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	UE_LOG(LogTemp, Warning, TEXT("InitGame %s %s"), *MapName, *Options);
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABasicGameModeBase::Tick(float dt) {
	UE_LOG(LogTemp, Warning, TEXT("Gameplay Tick"));
	//UE_LOG(LogTemp, Warning, TEXT("Tick"));

	/*

	TArray<AActor*> found_actors; 

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), found_actors);

	for (AActor* abstract_actor : found_actors)
	{
		AEnemy* enemy = Cast<AEnemy>(abstract_actor);

		if (enemy != nullptr) {
			//UE_LOG(LogTemp, Warning, TEXT("Enemy!"));
			auto location = enemy->GetActorLocation();

			UE_LOG(LogClass, Warning, TEXT("Enemy! %f %f %f"), location.X, location.Y, location.Z);

			//auto msg = FString::Printf(TEXT("Enemy! %f %f %f"), location.X, location.Y, location.Z);

			//UE_LOG(LogTemp, Warning, msg);
		}
	}
	*/
}

int32 ABasicGameModeBase::get_start_money() {
	return start_money;
}

float ABasicGameModeBase::get_money_increase() {
	return money_increase;
}

void ABasicGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

/*
void ABasicGameModeBase::RestartPlayer(AController* NewPlayer) {
		Super::RestartPlayer(NewPlayer);
		UE_LOG(LogTemp, Warning, TEXT("RestartPlayer play"));

		for (float i = -400; i < 600; i += 130) {
			auto pos = FVector(800, i, 0);

			AActor* my_actor = (AActor*)GetWorld()->SpawnActor(WeakEnemy, &pos);
		}

		auto pos = FVector(200, 0, 0);

		//AActor* spectator = (AActor*)GetWorld()->SpawnActor(DefaultPawnClass, &pos);
		//Contrller
		//RestartPlayer()
}
*/

void ABasicGameModeBase::restart_game() {
	UE_LOG(LogTemp, Warning, TEXT("Reset"));
	//RestartGame(); //Works, but not fully correctly

	//should_restart = true;

	EndMatch();

	SetMatchState(MatchState::WaitingToStart);

	UE_LOG(LogTemp, Warning, TEXT("End restart game"));
	//StartMatch()

	//ResetLevel();
}

void ABasicGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)  {
	UE_LOG(LogTemp, Warning, TEXT("PreLogin"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}
void ABasicGameModeBase::PostLogin(APlayerController* NewPlayer) {
	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	Super::PostLogin(NewPlayer);
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

void ABasicGameModeBase::HandleDisconnect
(
	UWorld* InWorld,
	UNetDriver* NetDriver
) {
	UE_LOG(LogTemp, Warning, TEXT("HandleDisconnect"));
	Super::HandleDisconnect(InWorld, NetDriver);
}

/** Called when the state transitions to InProgress */
void ABasicGameModeBase::HandleMatchHasStarted() {
	Super::HandleMatchHasStarted();
	on_start_match();
}

/** Called when the map transitions to WaitingPostMatch */
void ABasicGameModeBase::HandleMatchHasEnded() {
	UE_LOG(LogTemp, Warning, TEXT("HandleMatchHasEnded"));
	Super::HandleMatchHasEnded();

	on_clear_match();
	UE_LOG(LogTemp, Warning, TEXT("TODO:How long reset?"));
}

/** Called when the match transitions to LeavingMap */
void ABasicGameModeBase::HandleLeavingMap() {
	UE_LOG(LogTemp, Warning, TEXT("HandleLeavingMap"));
	Super::HandleLeavingMap();
}

/** Called when the match transitions to Aborted */
void ABasicGameModeBase::HandleMatchAborted() {
	UE_LOG(LogTemp, Warning, TEXT("HandleMatchAborted"));
	Super::HandleMatchAborted();
}

void ABasicGameModeBase::HandleMatchIsWaitingToStart() {
	UE_LOG(LogTemp, Warning, TEXT("HandleMatchIsWaitingToStart"));
	Super::HandleMatchIsWaitingToStart();

	StartMatch();
}

/*
void ABasicGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) {

}
*/

OptionPtr<ABasicGameStateBase> ABasicGameModeBase::get_game_state() {
	if (auto world = GetWorld()) {
		return OptionPtr<ABasicGameStateBase>::new_unchecked(world->GetGameState<ABasicGameStateBase>());
	}else {
		return OptionPtr<ABasicGameStateBase>::new_none();
	}
}

void ABasicGameModeBase::on_start_match() {
	UE_LOG(LogTemp, Warning, TEXT("Start match"));

	for (float i = -400; i < 600; i += 130) {
		auto pos = FVector(800, i, 0);

		AActor* my_actor = (AActor*)GetWorld()->SpawnActor(WeakEnemy, &pos);
	}
}

void ABasicGameModeBase::on_clear_match() {
	UE_LOG(LogTemp, Warning, TEXT("Clear match"));

	ResetLevel();

	UE_LOG(LogTemp, Warning, TEXT("End clear match"));
}