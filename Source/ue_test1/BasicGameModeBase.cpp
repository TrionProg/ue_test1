// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicGameModeBase.h"
#include "Spectator.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


ABasicGameModeBase::ABasicGameModeBase(): Super() {
	PrimaryActorTick.bCanEverTick = true;

	//Use C++ Spectator
	DefaultPawnClass = ASpectator::StaticClass(); 

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
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("InitGame"));

	for (float i = -400; i < 600; i+=130) {
		auto pos = FVector(800, i, 0);

		AActor* my_actor = (AActor*)GetWorld()->SpawnActor(WeakEnemy, &pos);
	}

	//Super::InitGame(MapName, Options, ErrorMessage);
}

void ABasicGameModeBase::Tick(float DeltaSeconds) {
	//UE_LOG(LogTemp, Warning, TEXT("Tick"));

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
}