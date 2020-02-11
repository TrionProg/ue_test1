// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicGameModeBase.h"
#include "Spectator.h"
#include "MyPlayerController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


ABasicGameModeBase::ABasicGameModeBase(): Super() {
	PrimaryActorTick.bCanEverTick = true;

	//Use C++ Spectator
	DefaultPawnClass = ASpectator::StaticClass(); 
	PlayerControllerClass = AMyPlayerController::StaticClass();

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
	//UE_LOG(LogTemp, Warning, TEXT("Gameplay Tick"));
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

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

	for (float i = -400; i < 600; i += 130) {
		auto pos = FVector(800, i, 0);

		AActor* my_actor = (AActor*)GetWorld()->SpawnActor(WeakEnemy, &pos);
	}
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
	//RestartGame();
	ResetLevel();
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